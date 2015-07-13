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
#include "caml/gc.h"
#include "caml/alloc.h"
#include "caml/memory.h"
#include "caml/misc.h"
#include "caml/mlvalues.h"


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
