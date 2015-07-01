#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * les appels d'allocations mémoires ne savent pas dans quel tas seront placé les données. */
char* heap1_start, heap2_start;
char* heap1_end, heap2_end;
int current_heap;
/* heap_ptr : pointeur du premier emplacement libre du tas
 * heap_end : pointeur de fin du tas courant */
char *heap_ptr, *heap_end;

/* some intern variables needed by the gc */
char* new_heap, *old_heap;


/* Version de Alloc_small pour le fichier interp.c (car nécessite sp d'être dans le scope)
/* TODO: Alloc_small est aussi appelée par des fichiers qui n'ont pas sp en scope,
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


/* Initialize the GC
 * This function MUST be called before the first allocation of the program
 * (ie in startup.c, around the begining of startup.c)
 * heap_size : the size of the heap. 2 heaps of this size will be allocated */
void caml_initialize_gc(int heap_size) {
  heap1_start = malloc(heap_size * sizeof value);
  heap1_end = heap1_start + heap_size * sizeof value;
  
  heap2_start = malloc(heap_size * sizeof value);
  heap2_end = heap2_start + heap_size * sizeof value;

  heap_ptr = heap1_start;
  heap_end = heap1_ptr + heap_size * sizeof value;
}


/* main GC function. (The one you need to call)
 * it does a garbage collection, and update the heap pointer
 * sp : current stack pointer in the interpretation loop (interp.c)
 *TODO: s'arrurer que début de pile = caml_extern_sp (cf stacks.c/h) 
 */
void caml_gc_collect(value *sp) {
  value* s_ptr; /* current stack_pointer */
  
  if (current_heap == 1) {
    new_heap = heap2_start;
    old_heap = heap1_start;
    heap_end = heap2_end;
    current_heap = 2;
  } else {
    new_heap = heap1_start;
    old_heap = heap2_start;
    heap_end = heap1_end;
    current_heap = 1;
  }
  
  for (s_ptr = caml_extern_sp; s_ptr != sp; s_ptr--) {
    caml_gc_one_value(s_ptr);
  }
  heap_ptr = new_heap;  
}

/* Garbage collect (is this a thing?) one value
 * (and call itsef recursivly if needed)
 */
/* Fonctionnement : (pour toi, lecteur francais)
 * si ptr pointe sur autre chose qu'un block : on ne fait rien.
 * sinon :
 *     - si le tag du block est >= No_scan_tag, on le block "bêtement" dans le nouveau tas,
 *               on met à jour son tag dans l'ancien tas, et on copie toutes ses données sans les lire
 *     - sinon, on copie tout dans le nouveau tas comme pour le cas précédent, puis
 *               on parcours chaque champ du block (dans le nouveau tas), et on appel cette fonction
 *               (caml_gc_one_value) sur chacun.
 */
/* TODO: un traitement particulier pour Infix_tag et Forward_tag ?? */
void caml_gc_one_value (value* ptr) {
  value v;
  header_t t;
  tag_t tag;
  mlsize_t sz;

  v = *ptr;
  if (Is_block (v)) {
    if (Hp_val(v) >= old_heap) { /* check if the block is in the heap
				  * (if not, then it's a global data, and we dont modify it) */
      hd = Hd_val(v);
      if (Is_white_hd(hd)) { /* the block has already been copied, so we juste need to change
			      * the reference */
	*ptr = Field(v, 0); continue;
      }
	
      tag = Tag_hd(hd);
      sz = Wosize_hd(hd);
      if (tag >= No_scan_tag) {
	memcpy(new_heap, hd, sizeof header_t);
	new_heap += sizeof header_t;
	memcpy(new_heap, v, sz * sizeof value);
	Field(v, 0) = new_heap;
	new_heap += sz * sizeof value;
	Hd_val(*ptr) = Whitehd_hd (h); /* The block has been copied, we must whitify the header */
      }
      
      else { /* tag < No_scan_tag */
	/* let's first copy the header and the data in the new heap */
	memcpy(new_heap, hd, sizeof header_t);
	new_heap += sizeof header_t;
	value *new_addr = new_heap;
	memcpy(new_heap, v, sz * sizeof value);
	Field(v, 0) = new_heap;
	new_heap += sz * sizeof value;	
	Hd_val(*ptr) = Whitehd_hd (h); /* Whitify the header now 
					* (in case of some reccursive block.. Is this even possible?) */
	
	/* And then, we can iterate on every field  */
	for (value i = 0; i < sz; i++) {
	  caml_gc_one_value((value*) ((new_heap - (sz * sizeof value)) + (int)i));
	}
	
	/* now we set Field(ptr, 0) to the new location of the block */
	Field(ptr, 0) = new_addr;
      }
      
    } /* Hp_val(v) >= old_heap */    
  } /* Is_block */
  
}
