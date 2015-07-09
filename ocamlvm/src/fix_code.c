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

/* Handling of blocks of bytecode (endianness switch, threading). */

#include <string.h>

#ifdef HAS_UNISTD
#include <unistd.h>
#endif

#include "caml/config.h"
#include "caml/fix_code.h"
#include "caml/instruct.h"
#include "caml/intext.h"
#include "caml/memory.h"
#include "caml/misc.h"
#include "caml/mlvalues.h"
#include "caml/reverse.h"

#define IS_BIG_ENDIAN (!(union { uint16_t u16; unsigned char c; }){ .u16 = 1 }.c)

code_t caml_start_code;
asize_t caml_code_size;
unsigned char * caml_saved_code;

/* Read the main bytecode block from a file */

static void caml_fixup_endianness(code_t code, asize_t len)
{
  if (IS_BIG_ENDIAN) {
    code_t p;
    len /= sizeof(opcode_t);
    for (p = code; p < code + len; p++) {
      Reverse_32(p, p);
    }
  }
}

void caml_init_code_fragments() {
  struct code_fragment * cf;
  /* Register the code in the table of code fragments */
  cf = caml_stat_alloc(sizeof(struct code_fragment));
  cf->code_start = (char *) caml_start_code;
  cf->code_end = (char *) caml_start_code + caml_code_size;
  caml_md5_block(cf->digest, caml_start_code, caml_code_size);
  cf->digest_computed = 1;
  caml_ext_table_init(&caml_code_fragments_table, 8);
  caml_ext_table_add(&caml_code_fragments_table, cf);
}

void caml_load_code(char* fd, asize_t len)
{
  int i;

  caml_code_size = len;
  caml_start_code = (code_t) caml_stat_alloc(caml_code_size);
  memcpy((char*) caml_start_code, fd, caml_code_size);
  caml_init_code_fragments();
  /* Prepare the code for execution */
  caml_fixup_endianness(caml_start_code, caml_code_size);
}


void caml_set_instruction(code_t pos, opcode_t instr)
{
  *pos = instr;
}

int caml_is_instruction(opcode_t instr1, opcode_t instr2)
{
  return instr1 == instr2;
}
