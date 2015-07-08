/***********************************************************************/
/*                                                                     */
/*                                OCaml                                */
/*                                                                     */
/*             Damien Doligez, projet Para, INRIA Rocquencourt         */
/*                                                                     */
/*  Copyright 1996 Institut National de Recherche en Informatique et   */
/*  en Automatique.  All rights reserved.  This file is distributed    */
/*  under the terms of the GNU Library General Public License, with    */
/*  the special exception on linking described in file ../LICENSE.     */
/*                                                                     */
/***********************************************************************/

#include <stdlib.h>
#include <string.h>
#include "caml/fail.h"
#include "caml/freelist.h"
#include "caml/gc.h"
#include "caml/memory.h"
#include "caml/misc.h"
#include "caml/mlvalues.h"
#include "caml/signals.h"

extern uintnat caml_percent_free;                   /* major_gc.c */

/* Page table management */

#define Page(p) ((uintnat) (p) >> Page_log)
#define Page_mask ((uintnat) -1 << Page_log)

#ifdef ARCH_SIXTYFOUR

/* 64-bit implementation:
   The page table is represented sparsely as a hash table
   with linear probing */

struct page_table {
  mlsize_t size;                /* size == 1 << (wordsize - shift) */
  int shift;
  mlsize_t mask;                /* mask == size - 1 */
  mlsize_t occupancy;
  uintnat * entries;            /* [size]  */
};

static struct page_table caml_page_table;

/* Page table entries are the logical 'or' of
   - the key: address of a page (low Page_log bits = 0)
   - the data: a 8-bit integer */

#define Page_entry_matches(entry,addr) \
  ((((entry) ^ (addr)) & Page_mask) == 0)

/* Multiplicative Fibonacci hashing
   (Knuth, TAOCP vol 3, section 6.4, page 518).
   HASH_FACTOR is (sqrt(5) - 1) / 2 * 2^wordsize. */
#ifdef ARCH_SIXTYFOUR
#define HASH_FACTOR 11400714819323198486UL
#else
#define HASH_FACTOR 2654435769UL
#endif
#define Hash(v) (((v) * HASH_FACTOR) >> caml_page_table.shift)

int caml_page_table_lookup(void * addr)
{
  uintnat h, e;

  h = Hash(Page(addr));
  /* The first hit is almost always successful, so optimize for this case */
  e = caml_page_table.entries[h];
  if (Page_entry_matches(e, (uintnat)addr)) return e & 0xFF;
  while(1) {
    if (e == 0) return 0;
    h = (h + 1) & caml_page_table.mask;
    e = caml_page_table.entries[h];
    if (Page_entry_matches(e, (uintnat)addr)) return e & 0xFF;
  }
}

int caml_page_table_initialize(mlsize_t bytesize)
{
  uintnat pagesize = Page(bytesize);

  caml_page_table.size = 1;
  caml_page_table.shift = 8 * sizeof(uintnat);
  /* Aim for initial load factor between 1/4 and 1/2 */
  while (caml_page_table.size < 2 * pagesize) {
    caml_page_table.size <<= 1;
    caml_page_table.shift -= 1;
  }
  caml_page_table.mask = caml_page_table.size - 1;
  caml_page_table.occupancy = 0;
  caml_page_table.entries = calloc(caml_page_table.size, sizeof(uintnat));
  if (caml_page_table.entries == NULL)
    return -1;
  else
    return 0;
}

static int caml_page_table_resize(void)
{
  struct page_table old = caml_page_table;
  uintnat * new_entries;
  uintnat i, h;

  caml_gc_message (0x08, "Growing page table to %lu entries\n",
                   caml_page_table.size);

  new_entries = calloc(2 * old.size, sizeof(uintnat));
  if (new_entries == NULL) {
    caml_gc_message (0x08, "No room for growing page table\n", 0);
    return -1;
  }

  caml_page_table.size = 2 * old.size;
  caml_page_table.shift = old.shift - 1;
  caml_page_table.mask = caml_page_table.size - 1;
  caml_page_table.occupancy = old.occupancy;
  caml_page_table.entries = new_entries;

  for (i = 0; i < old.size; i++) {
    uintnat e = old.entries[i];
    if (e == 0) continue;
    h = Hash(Page(e));
    while (caml_page_table.entries[h] != 0)
      h = (h + 1) & caml_page_table.mask;
    caml_page_table.entries[h] = e;
  }

  free(old.entries);
  return 0;
}

static int caml_page_table_modify(uintnat page, int toclear, int toset)
{
  uintnat h;

  Assert ((page & ~Page_mask) == 0);

  /* Resize to keep load factor below 1/2 */
  if (caml_page_table.occupancy * 2 >= caml_page_table.size) {
    if (caml_page_table_resize() != 0) return -1;
  }
  h = Hash(Page(page));
  while (1) {
    if (caml_page_table.entries[h] == 0) {
      caml_page_table.entries[h] = page | toset;
      caml_page_table.occupancy++;
      break;
    }
    if (Page_entry_matches(caml_page_table.entries[h], page)) {
      caml_page_table.entries[h] =
        (caml_page_table.entries[h] & ~toclear) | toset;
      break;
    }
    h = (h + 1) & caml_page_table.mask;
  }
  return 0;
}

#else

/* 32-bit implementation:
   The page table is represented as a 2-level array of unsigned char */

CAMLexport unsigned char * caml_page_table[Pagetable1_size];
static unsigned char caml_page_table_empty[Pagetable2_size] = { 0, };

int caml_page_table_initialize(mlsize_t bytesize)
{
  int i;
  for (i = 0; i < Pagetable1_size; i++)
    caml_page_table[i] = caml_page_table_empty;
  return 0;
}

static int caml_page_table_modify(uintnat page, int toclear, int toset)
{
  uintnat i = Pagetable_index1(page);
  uintnat j = Pagetable_index2(page);

  if (caml_page_table[i] == caml_page_table_empty) {
    unsigned char * new_tbl = calloc(Pagetable2_size, 1);
    if (new_tbl == 0) return -1;
    caml_page_table[i] = new_tbl;
  }
  caml_page_table[i][j] = (caml_page_table[i][j] & ~toclear) | toset;
  return 0;
}

#endif

int caml_page_table_add(int kind, void * start, void * end)
{
  uintnat pstart = (uintnat) start & Page_mask;
  uintnat pend = ((uintnat) end - 1) & Page_mask;
  uintnat p;

  for (p = pstart; p <= pend; p += Page_size)
    if (caml_page_table_modify(p, 0, kind) != 0) return -1;
  return 0;
}

int caml_page_table_remove(int kind, void * start, void * end)
{
  uintnat pstart = (uintnat) start & Page_mask;
  uintnat pend = ((uintnat) end - 1) & Page_mask;
  uintnat p;

  for (p = pstart; p <= pend; p += Page_size)
    if (caml_page_table_modify(p, kind, 0) != 0) return -1;
  return 0;
}

/* Useless now.
*/
char *caml_alloc_for_heap (asize_t request)
{
  return NULL;
}

/* Useless now.
*/
void caml_free_for_heap (char *mem)
{
  return;
}

/* Useless now.
*/
int caml_add_to_heap (char *m)
{
  return NULL;
}

/* Useless now.
*/
static char *expand_heap (mlsize_t request)
{
  return NULL;
}

/* Useless now.
*/
void caml_shrink_heap (char *chunk)
{
  return;
}

/* Should we remove that function? */
color_t caml_allocation_color (void *hp)
{
  return Caml_black;
}

/* No more Major Heap -> binding on caml_alloc_small */
CAMLexport value caml_alloc_shr (mlsize_t wosize, tag_t tag)
{
  return caml_alloc_small(wosize, tag);
}

/* Dependent memory is all memory blocks allocated out of the heap
   that depend on the GC (and finalizers) for deallocation.
   For the GC to take dependent memory into account when computing
   its automatic speed setting,
   you must call [caml_alloc_dependent_memory] when you alloate some
   dependent memory, and [caml_free_dependent_memory] when you
   free it.  In both cases, you pass as argument the size (in bytes)
   of the block being allocated or freed.
*/
CAMLexport void caml_alloc_dependent_memory (mlsize_t nbytes)
{
  caml_dependent_size += nbytes / sizeof (value);
  caml_dependent_allocated += nbytes / sizeof (value);
}

CAMLexport void caml_free_dependent_memory (mlsize_t nbytes)
{
  if (caml_dependent_size < nbytes / sizeof (value)){
    caml_dependent_size = 0;
  }else{
    caml_dependent_size -= nbytes / sizeof (value);
  }
}

/* Useless now.
*/
CAMLexport void caml_adjust_gc_speed (mlsize_t res, mlsize_t max)
{
  return;
}

/* This function doesn't do much since there isn't a major heap anymore*/
CAMLexport CAMLweakdef void caml_initialize (value *fp, value val)
{
  *fp = val;
}

/* This function doesn't do much since there isn't a major heap anymore*/
CAMLexport CAMLweakdef void caml_modify (value *fp, value val)
{
  *fp = val;
}

CAMLexport void * caml_stat_alloc (asize_t sz)
{
  void * result = malloc (sz);

  /* malloc() may return NULL if size is 0 */
  if (result == NULL && sz != 0) caml_raise_out_of_memory ();
  return result;
}

CAMLexport void caml_stat_free (void * blk)
{
  free (blk);
}

CAMLexport void * caml_stat_resize (void * blk, asize_t sz)
{
  void * result = realloc (blk, sz);

  if (result == NULL) caml_raise_out_of_memory ();
  return result;
}
