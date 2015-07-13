/***********************************************************************/
/*                                                                     */
/*                                OCaml                                */
/*                                                                     */
/*            Xavier Leroy, projet Cristal, INRIA Rocquencourt         */
/*                                                                     */
/*  Copyright 1996 Institut National de Recherche en Informatique et   */
/*  en Automatique.  All rights reserved.  This file is distributed    */
/*  under the terms of the GNU Library General Public License, with    */
/*  the special exception on linking described in file ../LICENSE.     */
/*                                                                     */
/***********************************************************************/

/* Operations on objects */

#include <string.h>
#include "caml/alloc.h"
#include "caml/fail.h"
#include "caml/gc.h"
#include "caml/interp.h"
#include "caml/memory.h"
#include "caml/misc.h"
#include "caml/mlvalues.h"
#include "caml/prims.h"

/* [size] is a value encoding a number of bytes */
CAMLprim value caml_static_alloc(value size)
{
  return (value) caml_stat_alloc((asize_t) Long_val(size));
}

CAMLprim value caml_static_free(value blk)
{
  caml_stat_free((void *) blk);
  return Val_unit;
}

CAMLprim value caml_static_resize(value blk, value new_size)
{
  return (value) caml_stat_resize((char *) blk, (asize_t) Long_val(new_size));
}

CAMLprim value caml_obj_is_block(value arg)
{
  return Val_bool(Is_block(arg));
}

CAMLprim value caml_obj_tag(value arg)
{
  if (Is_long (arg)){
    return Val_int (1000);   /* int_tag */
  }else if ((long) arg & (sizeof (value) - 1)){
    return Val_int (1002);   /* unaligned_tag */
  }else {
    return Val_int(Tag_val(arg));
  }
}

CAMLprim value caml_obj_set_tag (value arg, value new_tag)
{
  Tag_val (arg) = Int_val (new_tag);
  return Val_unit;
}

/* [size] is a value encoding a number of blocks */
CAMLprim value caml_obj_block(value tag, value size)
{
  value res;
  mlsize_t sz, i;
  tag_t tg;

  sz = Long_val(size);
  tg = Long_val(tag);
  if (sz == 0) return Atom(tg);
  res = caml_alloc(sz, tg);
  for (i = 0; i < sz; i++)
    Field(res, i) = Val_long(0);

  return res;
}

CAMLprim value caml_obj_dup(value arg)
{
  CAMLparam1 (arg);
  CAMLlocal1 (res);
  mlsize_t sz, i;
  tag_t tg;

  sz = Wosize_val(arg);
  if (sz == 0) CAMLreturn (arg);
  tg = Tag_val(arg);
  if (tg >= No_scan_tag) {
    res = caml_alloc(sz, tg);
    memcpy(Bp_val(res), Bp_val(arg), sz * sizeof(value));
  } else {
    res = caml_alloc_small(sz, tg);
    for (i = 0; i < sz; i++) Field(res, i) = Field(arg, i);
  }
  CAMLreturn (res);
}

/* Shorten the given block to the given size and return void.
   Raise Invalid_argument if the given size is less than or equal
   to 0 or greater than the current size.

   algorithm:
   Change the length field of the header.  Make up a white object
   with the leftover part of the object: this is needed in the major
   heap and harmless in the minor heap.

   [newsize] is a value encoding a number of words.
*/
CAMLprim value caml_obj_truncate (value v, value newsize)
{
  mlsize_t new_wosize = Long_val (newsize);
  header_t hd = Hd_val (v);
  tag_t tag = Tag_hd (hd);
  color_t color = Color_hd (hd);
  mlsize_t wosize = Wosize_hd (hd);
  mlsize_t i;

  if (tag == Double_array_tag) new_wosize *= Double_wosize;  /* PR#156 */

  if (new_wosize <= 0 || new_wosize > wosize){
    caml_invalid_argument ("Obj.truncate");
  }
  if (new_wosize == wosize) return Val_unit;
  /* PR#61: since we're about to lose our references to the elements
     beyond new_wosize in v, erase them explicitly so that the GC
     can darken them as appropriate. */
  if (tag < No_scan_tag) {
    for (i = new_wosize; i < wosize; i++){
      caml_modify(&Field(v, i), Val_unit);
    }
  }
  /* We must use an odd tag for the header of the leftovers so it does not
     look like a pointer because there may be some references to it in
     ref_table. */
  Field (v, new_wosize) =
    Make_header (Wosize_whsize (wosize-new_wosize), 1, Caml_white);
  Hd_val (v) = Make_header (new_wosize, tag, color);
  return Val_unit;
}

CAMLprim value caml_obj_add_offset (value v, value offset)
{
  return v + (unsigned long) Int32_val (offset);
}

/* The following functions are used in stdlib/lazy.ml.
   They are not written in OCaml because they must be atomic with respect
   to the GC.
 */

CAMLprim value caml_lazy_follow_forward (value v)
{
  if (Is_block (v) && Tag_val (v) == Forward_tag) {
    return Forward_val (v);
  } else {
    return v;
  }
}

CAMLprim value caml_lazy_make_forward (value v)
{
  CAMLparam1 (v);
  CAMLlocal1 (res);

  res = caml_alloc_small (1, Forward_tag);
  Field (res, 0) = v;
  CAMLreturn (res);
}

/* For mlvalues.h and camlinternalOO.ml
   See also GETPUBMET in interp.c
 */

CAMLprim value caml_get_public_method (value obj, value tag)
{
  value meths = Field (obj, 0);
  int li = 3, hi = Field(meths,0), mi;
  while (li < hi) {
    mi = ((li+hi) >> 1) | 1;
    if (tag < Field(meths,mi)) hi = mi-2;
    else li = mi;
  }
  /* return 0 if tag is not there */
  return (tag == Field(meths,li) ? Field (meths, li-1) : 0);
}


static value oo_last_id = Val_int(0);

CAMLprim value caml_set_oo_id (value obj) {
  Field(obj, 1) = oo_last_id;
  oo_last_id += 2;
  return obj;
}

CAMLprim value caml_int_as_pointer (value n) {
  return n - 1;
}
