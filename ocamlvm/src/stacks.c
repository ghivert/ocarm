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

/* To initialize and resize the stacks */

#include <string.h>
#include "caml/config.h"
#include "caml/fail.h"
#include "caml/misc.h"
#include "caml/mlvalues.h"
#include "caml/stacks.h"

CAMLexport value * caml_stack_low;
CAMLexport value * caml_stack_high;
CAMLexport value * caml_stack_threshold;
CAMLexport value * caml_extern_sp;
CAMLexport value * caml_trapsp;
CAMLexport value * caml_trap_barrier;
value caml_global_data = 0;

uintnat caml_max_stack_size;            /* also used in gc_ctrl.c */

void caml_init_stack (uintnat initial_max_size)
{
  caml_stack_low = (value *) caml_stat_alloc(Stack_size);
  caml_stack_high = caml_stack_low + Stack_size / sizeof (value);
  caml_stack_threshold = caml_stack_low + Stack_threshold / sizeof (value);
  caml_extern_sp = caml_stack_high;
  caml_trapsp = caml_stack_high;
  caml_trap_barrier = caml_stack_high + 1;
  caml_max_stack_size = initial_max_size;
}

void caml_realloc_stack(asize_t required_space)
{
  asize_t size;
  value * new_low, * new_high, * new_sp;
  value * p;

  size = caml_stack_high - caml_stack_low;
  do {
    if (size >= caml_max_stack_size) caml_raise_stack_overflow();
    size *= 2;
  } while (size < caml_stack_high - caml_extern_sp + required_space);
  new_low = (value *) caml_stat_alloc(size * sizeof(value));
  new_high = new_low + size;

#define shift(ptr) \
    ((char *) new_high - ((char *) caml_stack_high - (char *) (ptr)))

  new_sp = (value *) shift(caml_extern_sp);
  memmove((char *) new_sp,
          (char *) caml_extern_sp,
          (caml_stack_high - caml_extern_sp) * sizeof(value));
  caml_stat_free(caml_stack_low);
  caml_trapsp = (value *) shift(caml_trapsp);
  caml_trap_barrier = (value *) shift(caml_trap_barrier);
  for (p = caml_trapsp; p < new_high; p = Trap_link(p))
    Trap_link(p) = (value *) shift(Trap_link(p));
  caml_stack_low = new_low;
  caml_stack_high = new_high;
  caml_stack_threshold = caml_stack_low + Stack_threshold / sizeof (value);
  caml_extern_sp = new_sp;

#undef shift
}

