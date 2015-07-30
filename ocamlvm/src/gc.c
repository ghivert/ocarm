#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "caml/gc.h"
#include "caml/mlvalues.h"
#include "caml/stacks.h"
#include "caml/heap_pp.h"
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"

/* Only color used : black and white 
 * (it's ok to use only 2 colors since there is only one heap) */

/* heap1_start et heap2_start : le pointeur de début de chaque tas
 * heap1_end et heap2_end : le pointeur de fin de chaque tas
 * current_heap : 1 ou 2 selon le tas actif
 * les appels d'allocations mémoires ne savent pas dans quel tas seront placé les données.
 */
char *heap1_start, *heap2_start;
char *heap1_end, *heap2_end;
int current_heap;
char* new_heap_tailcall;
/* heap_ptr : pointeur du premier emplacement libre du tas
 * heap_end : pointeur de fin du tas courant */
char *heap_ptr, *heap_end;

/* some intern variables needed by the gc */
char *new_heap, *old_heap;
char* tab_heap_start[2];
char* tab_heap_end[2];

Gc_datas gc_datas;

/* Initialize the GC
 * This function MUST be called before the first allocation of the program
 * (ie in startup.c, around the begining of startup.c)
 * heap_size : the size of the heap. 2 heaps of this size will be allocated */
void caml_initialize_gc(int heap_size) {
  heap1_start = malloc(heap_size * sizeof (value));
  heap1_end = heap1_start + heap_size * sizeof (value);
  tab_heap_start[0] = heap1_start;
  tab_heap_end[0] = heap1_end;
  
  heap2_start = malloc(heap_size * sizeof (value));
  heap2_end = heap2_start + heap_size * sizeof (value);
  tab_heap_start[1] = heap2_start;
  tab_heap_end[1] = heap2_end;

  heap_ptr = heap1_start;
  heap_end = heap_ptr + heap_size * sizeof (value);

  current_heap = 0;
  if (heap1_start == 0) BSP_LED_On(LED3);
  if (heap2_start == 0) BSP_LED_On(LED4);
}

/* To be called just after global roots have been allocated */
void update_after_global_roots() {
  int added_size = (int)heap_ptr - (int)heap1_start;
  gc_datas.glob = (value*) heap1_start;
  memcpy(heap2_start, heap1_start, added_size);
  heap2_start = (char*)(added_size + (int)heap2_start);
  heap1_start = (char*)(added_size + (int)heap1_start);
}

/* print the heap. only used for debug */
void print_heap() {
  heap_pp(tab_heap_start[current_heap], (int)(heap_ptr - tab_heap_start[current_heap]),
	  "a name?", (int)(heap_ptr - tab_heap_start[current_heap]));
}


/* main GC function. (The one you need to call)
 * it does a garbage collection, and update the heap pointer
 * sp : current stack pointer in the interpretation loop (interp.c)
 *TODO: Ajouter les globals roots?
 */
void caml_gc_collect() {
  value* s_ptr; /* current stack_pointer */
  old_heap = tab_heap_start[current_heap % 2];
  heap_end = tab_heap_end[++current_heap % 2];
  new_heap = new_heap_tailcall = tab_heap_start[current_heap % 2];
  
  for (s_ptr = caml_stack_high; s_ptr != *gc_datas.sp; s_ptr--) {
    caml_gc_one_value(s_ptr);
  }
  for (s_ptr = gc_datas.glob; s_ptr < heap1_start; s_ptr++) {
    caml_gc_one_value(s_ptr);
  }

  caml_gc_one_value(gc_datas.accu);
  caml_gc_one_value(gc_datas.env);
  heap_ptr = new_heap;  
}

/* Garbage collect (is this a thing?) one value
 * Note: Unless the regular ocaml GC, we consider Forward_tag as a regular tag < No_scan_tag
 */

void caml_gc_one_value (value* ptr) {
  value v;
  header_t hd;
  tag_t tag;
  mlsize_t sz;
  int todo_tailcall = 0;
 
 start_collect :
  v = *ptr;

  if (Is_block (v)) {
    if (Hp_val(v) >= old_heap) { /* check if the block is in the heap
				  * (if not, then it's a global data, and we dont modify it) */
      hd = Hd_val(v);
      if (Is_white_hd(hd)) { /* the block has already been copied, so we juste need to update
			      * the reference */
	*ptr = Field(v, 0); goto tail_call;
      }
	
      tag = Tag_hd(hd);
      sz = Wosize_hd(hd);
      if (tag >= No_scan_tag) {
	memcpy(new_heap, (void*)hd, sizeof (header_t));
	new_heap += sizeof (header_t);
	memcpy(new_heap,  (void*)v, sz * sizeof (value));
	Field(v, 0) = (value)new_heap;
	new_heap += sz * sizeof (value);
	Hd_val(*ptr) = Whitehd_hd (hd); /* The block has been copied, we must whitify the header */
      }

      else if (tag == Infix_tag) {
	value start = v - Infix_offset_hd(hd);
	if (Is_white_val(start)) {
	  *ptr = Field(start, 0) + Infix_offset_hd(hd);
	} else {
	  caml_gc_one_value(&start);
	  *ptr = Field(start, 0) + Infix_offset_hd(hd);
	}
      }
      
      else { /* tag < No_scan_tag */
	/* let's first copy the header and the data in the new heap */
	memcpy(new_heap,  (void*)hd, sizeof (header_t));
	new_heap += sizeof (header_t);
	value *new_addr = (value*)new_heap;
	memcpy(new_heap,  (void*)v, sz * sizeof (value));
	new_heap += sz * sizeof (value);	
	
	Hd_val(*ptr) = Whitehd_hd (hd);
	Field(ptr, 0) = (value)new_addr;
	
      }      
    } /* Hp_val(v) >= old_heap */    
  } /* Is_block */

 tail_call:
  if (new_heap_tailcall == new_heap)
    return;
  if (todo_tailcall == 0) {
    header_t hd_loc = Hd_val(*new_heap_tailcall);
    todo_tailcall = Wosize_hd(hd_loc);
    if (Tag_hd(hd_loc) >= No_scan_tag) {
      new_heap_tailcall += (todo_tailcall + 1); // +1 for the header
      todo_tailcall = 0;
      goto tail_call;
    }
  }
  ptr = ++new_heap_tailcall;
  todo_tailcall--;
  goto start_collect;

}
