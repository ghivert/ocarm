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

#ifndef CAML_GC_H
#define CAML_GC_H


#include "mlvalues.h"

extern char *heap_ptr;
extern char *heap_end;

#define Caml_white (0 << 8)
#define Caml_black (3 << 8)

#define Color_hd(hd) ((color_t) ((hd) & Caml_black))
#define Color_hp(hp) (Color_hd (Hd_hp (hp)))
#define Color_val(val) (Color_hd (Hd_val (val)))

#define Is_white_hd(hd) (Color_hd (hd) == Caml_white)
#define Is_black_hd(hd) (Color_hd (hd) == Caml_black)

#define Whitehd_hd(hd) (((hd)  & ~Caml_black)/*| Caml_white*/)
#define Blackhd_hd(hd) (((hd)/*& ~Caml_black*/)| Caml_black)

/* This depends on the layout of the header.  See [mlvalues.h]. */
#define Make_header(wosize, tag, color)                                       \
       ((header_t) (((header_t) (wosize) << 10)                               \
                    + (color)                                                 \
                    + (tag_t) (tag)))                                       

#define Is_white_val(val) (Color_val(val) == Caml_white)
#define Is_black_val(val) (Color_val(val) == Caml_black)

/* For extern.c */
#define Colornum_hd(hd) ((color_t) (((hd) >> 8) & 3))
#define Coloredhd_hd(hd,colnum) (((hd) & ~Caml_black) | ((colnum) << 8))

typedef struct {
  value* accu;
  value** sp;
  value* env;
  value* glob;
} Gc_datas;

extern Gc_datas gc_datas;

void update_after_global_roots();
void caml_initialize_gc(int heap_size);
void caml_gc_collect();
void caml_gc_one_value (value* ptr);
void print_heap();

#endif /* CAML_GC_H */
