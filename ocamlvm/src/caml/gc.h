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
#define Caml_gray  (1 << 8)
#define Caml_blue  (2 << 8)
#define Caml_black (3 << 8)

#define Color_hd(hd) ((color_t) ((hd) & Caml_black))
#define Color_hp(hp) (Color_hd (Hd_hp (hp)))
#define Color_val(val) (Color_hd (Hd_val (val)))

#define Is_white_hd(hd) (Color_hd (hd) == Caml_white)
#define Is_gray_hd(hd) (Color_hd (hd) == Caml_gray)
#define Is_blue_hd(hd) (Color_hd (hd) == Caml_blue)
#define Is_black_hd(hd) (Color_hd (hd) == Caml_black)

#define Whitehd_hd(hd) (((hd)  & ~Caml_black)/*| Caml_white*/)
#define Grayhd_hd(hd)  (((hd)  & ~Caml_black)  | Caml_gray)
#define Blackhd_hd(hd) (((hd)/*& ~Caml_black*/)| Caml_black)
#define Bluehd_hd(hd)  (((hd)  & ~Caml_black)  | Caml_blue)

/* This depends on the layout of the header.  See [mlvalues.h]. */
#define Make_header(wosize, tag, color)                                       \
      (/*Assert ((wosize) <= Max_wosize),*/                                   \
       ((header_t) (((header_t) (wosize) << 10)                               \
                    + (color)                                                 \
                    + (tag_t) (tag)))                                         \
      )

#define Is_white_val(val) (Color_val(val) == Caml_white)
#define Is_gray_val(val) (Color_val(val) == Caml_gray)
#define Is_blue_val(val) (Color_val(val) == Caml_blue)
#define Is_black_val(val) (Color_val(val) == Caml_black)

/* For extern.c */
#define Colornum_hd(hd) ((color_t) (((hd) >> 8) & 3))
#define Coloredhd_hd(hd,colnum) (((hd) & ~Caml_black) | ((colnum) << 8))

/* Version de Alloc_small pour le fichier interp.c (car nécessite sp d'être dans le scope)
 * TODO: Alloc_small est aussi appelée par des fichiers qui n'ont pas sp en scope,
 * il faut donc trouver une solution. (qui peut être de déclarer value sp = NULL 
 * dans ces fichiers, ou bien de faire une autre macro pour ces fichiers, 
 * ou bien de rajouter sp en paramètre de Alloc_small, et de tester si sp est NULL ou non) */
#define Alloc_small(result, wosize, tag) do {				\
    if (heap_ptr + (Bhsize_wosize(wosize)) > heap_end) {		\
      caml_gc_collect(sp);						\
    }									\
    Hd_hp (heap_ptr) = Make_header ((wosize), (tag), Caml_black);       \
    (result) = Val_hp (heap_ptr);					\
    heap_ptr += Bhsize_wosize(wosize);					\
  }while(0)

void caml_initialize_gc(int heap_size);
void caml_gc_collect(value *sp);
void caml_gc_one_value (value* ptr);


#endif /* CAML_GC_H */
