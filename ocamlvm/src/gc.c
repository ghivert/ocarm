#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "caml/gc.h"
#include "caml/mlvalues.h"
#include "caml/stacks.h"

/* petite intro (pour toi, lecteur francais) :
 * toutes les allocations mémoires de interp.c passent maintenant par ce fichier,
 * dans la mesure où : l'adresse du premier emplacement libre du tas est dans ce fichier,
 * la fonction de garbage collection est ici aussi.
 * (c'est censé être suffisant, non?) */


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
/* heap_ptr : pointeur du premier emplacement libre du tas
 * heap_end : pointeur de fin du tas courant */
char *heap_ptr, *heap_end;

/* some intern variables needed by the gc */
char *new_heap, *old_heap;
char tab_heap_start[2];
char tab_heap_end[2];

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
}

/* To be called just after global roots have been allocated */
void update_after_global_roots() {
  char* added_size = heap_ptr - heap1_start;
  memcpy(heap2_start, heap1_start, added_size);
  heap2_start += added_size;
  heap1_start += added_size;
  
}


/* main GC function. (The one you need to call)
 * it does a garbage collection, and update the heap pointer
 * sp : current stack pointer in the interpretation loop (interp.c)
 *TODO: s'arrurer que début de pile = caml_extern_sp (cf stacks.c/h) 
 * Ajouter les globals roots?
 */
void caml_gc_collect() {
  value* s_ptr; /* current stack_pointer */
  old_heap = tab_heap_start[current_heap % 2];
  heap_end = tab_heap_end[++current_heap % 2];
  new_heap = tab_heap_start[current_heap % 2];
  
  /*if (current_heap == 1) {
    new_heap = heap2_start;
    old_heap = heap1_start;
    heap_end = heap2_end;
    current_heap = 2;
  } else {
    new_heap = heap1_start;
    old_heap = heap2_start;
    heap_end = heap1_end;
    current_heap = 1;
    } */
  
  for (s_ptr = caml_extern_sp; s_ptr != *gc_datas.sp; s_ptr--) {
    caml_gc_one_value(s_ptr);
  }
  caml_gc_one_value(*gc_datas.accu);
  caml_gc_one_value(*gc_datas.env);
  heap_ptr = new_heap;  
}

/* Garbage collect (is this a thing?) one value
 * (and call itsef recursivly if needed)
 * Note: Unless the regular ocaml GC, we consider Forward_tag as a regular tag < No_scan_tag
 */
/* Fonctionnement : (pour toi, lecteur francais)
 * si ptr pointe sur autre chose qu'un block : on ne fait rien.
 * sinon :
 *     - si le tag du block est >= No_scan_tag, on le block "bêtement" dans le nouveau tas,
 *               on met à jour son tag dans l'ancien tas, et on copie toutes ses données sans les lire
 *     - si c'est un Infix_tag, on check si la fermeture "englobante" a déjà été copiée,
 *          si oui on met à jour le pointeur, si non, on la copie puis on met à jour le pointeur
 *     - sinon, on copie tout dans le nouveau tas comme pour le cas précédent, puis
 *               on parcours chaque champ du block (dans le nouveau tas), et on appel cette fonction
 *               (caml_gc_one_value) sur chacun.
 */
void caml_gc_one_value (value* ptr) {
  value v;
  header_t hd;
  tag_t tag;
  mlsize_t sz;

  v = *ptr;
  if (Is_block (v)) {
    if (Hp_val(v) >= old_heap) { /* check if the block is in the heap
				  * (if not, then it's a global data, and we dont modify it) */
      hd = Hd_val(v);
      if (Is_white_hd(v)) { /* the block has already been copied, so we juste need to change
			      * the reference */
	*ptr = Field(v, 0); return;
      }
	
      tag = Tag_hd(hd);
      sz = Wosize_hd(hd);
      if (tag >= No_scan_tag) {
	memcpy(new_heap, (void*)hd, sizeof (header_t));
	new_heap += sizeof (header_t);
	memcpy(new_heap,  (void*)v, sz * sizeof (value));
	Field(v, 0) = new_heap;
	new_heap += sz * sizeof (value);
	Hd_val(*ptr) = Whitehd_hd (hd); /* The block has been copied, we must whitify the header */
      }

      else if (tag == Infix_tag) {
	value start = v - Infix_offset_hd(hd);
	if (Is_white_hd(start)) {
	  *ptr = Field(start, 0) + Infix_offset_hd(hd);
	} else {
	  caml_gc_one_value(start);
	  *ptr = Field(start, 0) + Infix_offset_hd(hd);
	}
      }
      
      else { /* tag < No_scan_tag */
	/* let's first copy the header and the data in the new heap */
	memcpy(new_heap,  (void*)hd, sizeof (header_t));
	new_heap += sizeof (header_t);
	value *new_addr = new_heap;
	memcpy(new_heap,  (void*)v, sz * sizeof (value));
	new_heap += sz * sizeof (value);	
	
	Hd_val(*ptr) = Whitehd_hd (hd); 
	/* now we set Field(ptr, 0) to the new location of the block */
	Field(ptr, 0) = new_addr;

	/* And then, we can iterate on every field  */
	for (value i = 0; i < sz; i++) {
	  caml_gc_one_value((value*) ((new_heap - (sz * sizeof (value))) + (int)i));
	}
      }
      
    } /* Hp_val(v) >= old_heap */    
  } /* Is_block */
  
}
