/***********************************************************************/
/*                                                                     */
/*                                OCaml                                */
/*                                                                     */
/*         Xavier Leroy and Damien Doligez, INRIA Rocquencourt         */
/*                                                                     */
/*  Copyright 1996 Institut National de Recherche en Informatique et   */
/*  en Automatique.  All rights reserved.  This file is distributed    */
/*  under the terms of the GNU Library General Public License, with    */
/*  the special exception on linking described in file ../LICENSE.     */
/*                                                                     */
/***********************************************************************/

/* Start-up code */

#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>

#ifdef HAS_UNISTD
#include <unistd.h>
#endif

#include "caml/alloc.h"
#include "caml/config.h"
#include "caml/custom.h"
#include "caml/gc.h"
#include "caml/exec.h"
#include "caml/fail.h"
#include "caml/fix_code.h"
#include "caml/interp.h"
#include "caml/intext.h"
#include "caml/memory.h"
#include "caml/misc.h"
#include "caml/mlvalues.h"
#include "caml/prims.h"
#include "caml/reverse.h"
#include "caml/stacks.h"
#include "caml/startup.h"
#include "caml/version.h"
#include "caml/prims.h"



#include "bytecode.h"
/* const int bytecode_length; 
 * const char bytecode[] = {'a', 'b', 'c'}; */

extern int caml_parser_trace;

CAMLexport header_t caml_atom_table[256];

/* Initialize the atom table */

static void init_atoms(void)
{
  int i;
  for(i = 0; i < 256; i++) caml_atom_table[i] = Make_header(0, i, Caml_white);
}


static void fixup_endianness_trailer(uint32_t * p)
{
  if (!IS_BIG_ENDIAN)
    Reverse_32(p, p);
}

static int read_trailer(struct exec_trailer *trail)
{
  /*if (lseek(fd, (long) -TRAILER_SIZE, SEEK_END) == -1) // offset 16 bytes avant la fin
   * return BAD_BYTECODE; */
  char* fd = bytecode + bytecode_length - 1 - TRAILER_SIZE;
  /*if (read(fd, (char *) trail, TRAILER_SIZE) < TRAILER_SIZE) // lit 16 bytes et les met dans trail
   * return BAD_BYTECODE; */
  memcpy(trail, fd, TRAILER_SIZE);
  fixup_endianness_trailer(&trail->num_sections); // inverse trail si nécessaire
  if (strncmp(trail->magic, EXEC_MAGIC, 12) == 0)
    return 0;
  else
    return BAD_BYTECODE;
}

int caml_attempt_open(struct exec_trailer *trail)
{
  int err;
  char* fd = bytecode;
  
  err = read_trailer(trail);
  if (err != 0)
    return err;
  
  return (int) fd;
}

/* Read the section descriptors */

void caml_read_section_descriptors(struct exec_trailer *trail)
{
  int toc_size;
  char* fd = bytecode;
  toc_size = trail->num_sections * 8;
  trail->section = malloc(toc_size);
  //lseek(fd, - (long) (TRAILER_SIZE + toc_size), SEEK_END);
  fd += bytecode_length - 1 - (long)(TRAILER_SIZE + toc_size);
  /* if (read(fd, (char *) trail->section, toc_size) != toc_size)
   *  fprintf(stderr, "Fatal error: cannot read section table\n"); fflush(stderr);
   */
  memcpy((char*)trail->section, fd, toc_size);
  /* Fixup endianness of lengths */
  for (uint32_t i = 0; i < trail->num_sections; i++)
    fixup_endianness_trailer(&(trail->section[i].len));
}

/* Position fd at the beginning of the section having the given name.
   Return the length of the section data in bytes, or -1 if no section
   found with that name. */

int32_t caml_seek_optional_section(char** fd, struct exec_trailer *trail, char *name)
{
  long ofs;
  int i;

  ofs = TRAILER_SIZE + trail->num_sections * 8;
  for (i = trail->num_sections - 1; i >= 0; i--) {
    ofs += trail->section[i].len;
    if (strncmp(trail->section[i].name, name, 4) == 0) {
      //lseek(fd, -ofs, SEEK_END);
      *fd = bytecode + bytecode_length - 1 - ofs;
      return trail->section[i].len;
    }
  }
  return -1;
}

/* Position fd at the beginning of the section having the given name.
   Return the length of the section data in bytes. */

int32_t caml_seek_section(char** fd, struct exec_trailer *trail, char *name)
{
  int32_t len = caml_seek_optional_section(fd, trail, name);
  if (len == -1)
    fprintf(stderr,"Fatal_error: section `%s' is missing\n", name);
  return len;
}

/* Read and return the contents of the section having the given name.
   Add a terminating 0.  Return NULL if no such section. */

static char * read_section(struct exec_trailer *trail, char *name)
{
  int32_t len;
  char * data;
  char* fd;

  len = caml_seek_optional_section(&fd, trail, name);
  if (len == -1) return NULL;
  data = malloc(len + 1);
  memcpy(data, fd, len);
  /*if (read(fd, data, len) != len)
   * fprintf(stderr, "Fatal error: error reading section %s\n", name);*/
  data[len] = 0;
  return data;
}


/* Configuration parameters and flags */


extern void caml_init_ieee_floats (void);
struct ext_table caml_prim_table;
/* Main entry point */

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    //Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    //Error_Handler();
  }
}

CAMLexport void caml_main(char **argv)
{
  HAL_Init();
  SystemClock_Config();
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);
  BSP_LED_Init(LED7);
  BSP_LED_Init(LED8);
  BSP_LED_Init(LED9);
  BSP_LED_Init(LED10);
  char* fd;
  int pos;
  struct exec_trailer trail; // struct définie dans exec.h
  value res;
  char * shared_lib_path, * shared_libs, * req_prims;

  /* Machine-dependent initialization of the floating-point hardware
     so that it behaves as much as possible as specified in IEEE */
  caml_init_ieee_floats();
  caml_init_custom_operations();
  //caml_ext_table_init(&caml_shared_libs_path, 8);
  caml_external_raise = NULL;
  pos = 0;

  fd = (char *) caml_attempt_open(&trail); // lit le nombre de sections et le magic word
  char* tmp = bytecode;
  /* Read the table of contents (section descriptors) */
  caml_read_section_descriptors(&trail); // lit le nom et la taille des sections
  /* Initialize the abstract machine */
  caml_initialize_gc (Heap_size); // définit dans gc.c
  caml_init_stack (Max_stack_def); // définit dans stacks.c
  init_atoms(); // ?!?
  /* Load the code */
  caml_code_size = caml_seek_section(&fd, &trail, "CODE"); // positionne fd au début de la section CODE, et renvoie la taille de cette section
  caml_load_code(fd, caml_code_size); // dans fix_code.c : lit la section CODE, et stock le pointeur de début dans la variable (globale) caml_code_fragments_table et de fin
	// et fais des "machins" pour les goto calculés
  /* Build the table of primitives */
  /* shared_lib_path = read_section(&trail, "DLPT"); // lit la section DLPT */
  /* shared_libs = read_section(&trail, "DLLS"); // lit la section DLLS */
  req_prims = read_section(&trail, "PRIM"); // lit la section PRIM
  /* if (req_prims == NULL) caml_fatal_error("Fatal error: no PRIM section\n"); */
  caml_build_primitive_table(NULL, NULL, req_prims); // dans dynlink.c :
  /* 	// remplis les variables globales caml_shared_libs_path, caml_prim_table */
  /* 	// fonctions de manipultation des tables : voir misc.c */
  /* 	// Et ouvre les librairies partagées */
  /* caml_stat_free(shared_lib_path); // caml_stat_free == free */
  /* caml_stat_free(shared_libs); */
  caml_stat_free(req_prims);
  /* Load the globals */
  caml_seek_section(&fd, &trail, "DATA"); // positionne fd au début de la section DATA
  caml_global_data = caml_input_val(fd); // dans intern.c :
  //BSP_LED_On(LED6);
  update_after_global_roots();
  caml_stat_free(trail.section);

  /* Execute the program */
  res = caml_interprete(caml_start_code, caml_code_size); // lance l'interprete sur le code
  printf("Execution terminée.\n");
  /* ************************************************************************************* */
  /* Changer le traitement du cas d'erreur? genre faire clignoter une LED si ca a planté ? */
  /* ************************************************************************************* */
}
