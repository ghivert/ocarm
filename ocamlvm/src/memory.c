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
