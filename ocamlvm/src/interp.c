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

/* The bytecode interpreter */
#include <stdio.h>
#include <stdint.h>

#include "caml/alloc.h"
#include "caml/fail.h"
#include "caml/fix_code.h"
#include "caml/instruct.h"
#include "caml/interp.h"
#include "caml/memory.h"
#include "caml/misc.h"
#include "caml/mlvalues.h"
#include "caml/prims.h"
#include "caml/stacks.h"
#include "caml/gc.h"
#include "caml/dynlink.h"

/* Registers for the abstract machine:
   pc         the code pointer
   sp         the stack pointer (grows downward)
   accu       the accumulator
   env        heap-allocated environment
   caml_trapsp pointer to the current trap frame
   extra_args number of extra arguments provided by the caller

   sp is a local copy of the global variable caml_extern_sp. */

/* Instruction decoding */

#define Instruct(name) case name
#define Next break


#define VERBOSE

#define Setup_for_c_call				\
  { saved_pc = pc; *--sp = env; caml_extern_sp = sp; }
#define Restore_after_c_call					\
  { sp = caml_extern_sp; env = *sp++; saved_pc = NULL; }


/* The interpreter itself */

value caml_interprete(code_t prog, asize_t prog_size)
{
  register code_t pc;
  value * sp;
  value accu;
  
  value env;
  intnat extra_args;
  struct longjmp_buffer * initial_external_raise;
  int initial_sp_offset;
  volatile code_t saved_pc = NULL;
  struct longjmp_buffer raise_buf;

  opcode_t curr_instr;

  if (prog == NULL) {           /* Interpreter is initializing */
    return Val_unit;
  }
  
  /* Initialising the struct for gc */
  gc_datas.sp = &sp;
  gc_datas.env = &env;
  gc_datas.accu = &accu;

  
  initial_sp_offset = (char *) caml_stack_high - (char *) caml_extern_sp;
  initial_external_raise = caml_external_raise;
  saved_pc = NULL;

  if (sigsetjmp(raise_buf.buf, 0)) {
    sp = caml_extern_sp;
    accu = caml_exn_bucket;
    pc = saved_pc; saved_pc = NULL;
    if (pc != NULL) pc += 2;
    /* +2 adjustement for the sole purpose of backtraces */
    goto raise_exception;
  }
  caml_external_raise = &raise_buf;

  sp = caml_extern_sp;
  pc = prog;
  extra_args = 0;
  env = Atom(0);
  accu = Val_int(0);
  int cpt = 1;
  while(1) {
#ifdef VERBOSE
    printf("Iter %d --- ", cpt++);
#endif
    curr_instr = *pc++;
    
    switch(curr_instr) {
      
      /* Basic stack operations */
      
    Instruct(ACC0):
#ifdef VERBOSE
      printf("ACC0\n");
#endif
      accu = sp[0]; Next;
    Instruct(ACC1):
#ifdef VERBOSE
      printf("ACC1\n");
#endif
      accu = sp[1]; Next;
    Instruct(ACC2):
#ifdef VERBOSE
      printf("ACC2\n");
#endif
      accu = sp[2]; Next;
    Instruct(ACC3):
#ifdef VERBOSE
      printf("ACC3\n");
#endif
      accu = sp[3]; Next;
    Instruct(ACC4):
#ifdef VERBOSE
      printf("ACC4\n");
#endif
      accu = sp[4]; Next;
    Instruct(ACC5):
#ifdef VERBOSE
      printf("ACC5\n");
#endif
      accu = sp[5]; Next;
    Instruct(ACC6):
#ifdef VERBOSE
      printf("ACC6\n");
#endif
      accu = sp[6]; Next;
    Instruct(ACC7):
#ifdef VERBOSE
      printf("ACC7\n");
#endif
      accu = sp[7]; Next;

    Instruct(PUSH): Instruct(PUSHACC0):
#ifdef VERBOSE
      printf("PUSHACC0\n");
#endif
      *--sp = accu; Next;
    Instruct(PUSHACC1):
      #ifdef VERBOSE
      printf("PUSHACC1\n");
#endif
      *--sp = accu; accu = sp[1]; Next;
    Instruct(PUSHACC2):
#ifdef VERBOSE
      printf("PUSHACC2\n");
#endif
      *--sp = accu; accu = sp[2]; Next;
    Instruct(PUSHACC3):
#ifdef VERBOSE
      printf("PUSHACC3\n");
#endif
      *--sp = accu; accu = sp[3]; Next;
    Instruct(PUSHACC4):
#ifdef VERBOSE
      printf("PUSHACC4\n");
#endif
      *--sp = accu; accu = sp[4]; Next;
    Instruct(PUSHACC5):
#ifdef VERBOSE
      printf("PUSHACC5\n");
#endif
      *--sp = accu; accu = sp[5]; Next;
    Instruct(PUSHACC6):
#ifdef VERBOSE
      printf("PUSHACC6\n");
#endif
      *--sp = accu; accu = sp[6]; Next;
    Instruct(PUSHACC7):
#ifdef VERBOSE
      printf("PUSHACC7\n");
#endif
      *--sp = accu; accu = sp[7]; Next;

    Instruct(PUSHACC):
#ifdef VERBOSE
      printf("PUSH");
#endif
      *--sp = accu;
      /* Fallthrough */
    Instruct(ACC):
#ifdef VERBOSE
      printf("ACC %d\n", *pc);
#endif
      accu = sp[*pc++];
      Next;

    Instruct(POP):
#ifdef VERBOSE
      printf("POP %d\n", *pc);
#endif
      sp += *pc++;
      Next;
    Instruct(ASSIGN):
#ifdef VERBOSE
      printf("ASSIGN %d\n", *pc);
#endif
      sp[*pc++] = accu;
      accu = Val_unit;
      Next;

      /* Access in heap-allocated environment */

    Instruct(ENVACC1):
#ifdef VERBOSE
      printf("ENVACC1\n");
#endif
      accu = Field(env, 1); Next;
    Instruct(ENVACC2):
#ifdef VERBOSE
      printf("ENVACC2\n");
#endif
      accu = Field(env, 2); Next;
    Instruct(ENVACC3):
#ifdef VERBOSE
      printf("ENVACC3\n");
#endif
      accu = Field(env, 3); Next;
    Instruct(ENVACC4):
#ifdef VERBOSE
      printf("ENVACC4\n");
#endif
      accu = Field(env, 4); Next;

    Instruct(PUSHENVACC1):
#ifdef VERBOSE
      printf("PUSHENVACC1\n");
#endif
      *--sp = accu; accu = Field(env, 1); Next;
    Instruct(PUSHENVACC2):
#ifdef VERBOSE
      printf("PUSHENVACC2\n");
#endif
      *--sp = accu; accu = Field(env, 2); Next;
    Instruct(PUSHENVACC3):
#ifdef VERBOSE
      printf("PUSHENVACC3\n");
#endif
      *--sp = accu; accu = Field(env, 3); Next;
    Instruct(PUSHENVACC4):
#ifdef VERBOSE
      printf("PUSHENVACC4\n");
#endif
      *--sp = accu; accu = Field(env, 4); Next;

    Instruct(PUSHENVACC):
#ifdef VERBOSE
      printf("PUSH");
#endif
      *--sp = accu;
      /* Fallthrough */
    Instruct(ENVACC):
#ifdef VERBOSE
      printf("ENVACC %d\n", *pc);
#endif
      accu = Field(env, *pc++);
      Next;

      /* Function application */

    Instruct(PUSH_RETADDR): 
#ifdef VERBOSE
      printf("PUSH_RETADDR\n");
#endif
      {
	sp -= 3;
	sp[0] = (value) (pc + *pc);
	sp[1] = env;
	sp[2] = Val_long(extra_args);
	pc++;
	Next;
      }
    Instruct(APPLY): 
#ifdef VERBOSE
      printf("APPLY %d\n", *pc);
#endif
      {
	extra_args = *pc - 1;
	pc = Code_val(accu);
	env = accu;
	goto check_stacks;
      }
    Instruct(APPLY1): 
#ifdef VERBOSE
      printf("APPLY1\n");
#endif
{
	value arg1 = sp[0];
	sp -= 3;
	sp[0] = arg1;
	sp[1] = (value)pc;
	sp[2] = env;
	sp[3] = Val_long(extra_args);
	pc = Code_val(accu);
	env = accu;
	extra_args = 0;
	goto check_stacks;
      }
    Instruct(APPLY2): 
#ifdef VERBOSE
      printf("APPLY2\n");
#endif
{
	value arg1 = sp[0];
	value arg2 = sp[1];
	sp -= 3;
	sp[0] = arg1;
	sp[1] = arg2;
	sp[2] = (value)pc;
	sp[3] = env;
	sp[4] = Val_long(extra_args);
	pc = Code_val(accu);
	env = accu;
	extra_args = 1;
	goto check_stacks;
      }
    Instruct(APPLY3): 
#ifdef VERBOSE
      printf("APPLY3\n");
#endif
{
	value arg1 = sp[0];
	value arg2 = sp[1];
	value arg3 = sp[2];
	sp -= 3;
	sp[0] = arg1;
	sp[1] = arg2;
	sp[2] = arg3;
	sp[3] = (value)pc;
	sp[4] = env;
	sp[5] = Val_long(extra_args);
	pc = Code_val(accu);
	env = accu;
	extra_args = 2;
	goto check_stacks;
      }

    Instruct(APPTERM): 
#ifdef VERBOSE
 printf("APPLYTERM %d %d\n", *pc, *(pc+1));
#endif
{
	int nargs = *pc++;
	int slotsize = *pc;
	value * newsp;
	int i;
	/* Slide the nargs bottom words of the current frame to the top
	   of the frame, and discard the remainder of the frame */
	newsp = sp + slotsize - nargs;
	for (i = nargs - 1; i >= 0; i--) newsp[i] = sp[i];
	sp = newsp;
	pc = Code_val(accu);
	env = accu;
	extra_args += nargs - 1;
	goto check_stacks;
      }
    Instruct(APPTERM1): 
#ifdef VERBOSE
 printf("APPTERM1 %d\n", *pc);
#endif
{
	value arg1 = sp[0];
	sp = sp + *pc - 1;
	sp[0] = arg1;
	pc = Code_val(accu);
	env = accu;
	goto check_stacks;
      }
    Instruct(APPTERM2): 
#ifdef VERBOSE
 printf("APPTERM2 %d\n", *pc);
#endif
{
	value arg1 = sp[0];
	value arg2 = sp[1];
	sp = sp + *pc - 2;
	sp[0] = arg1;
	sp[1] = arg2;
	pc = Code_val(accu);
	env = accu;
	extra_args += 1;
	goto check_stacks;
      }
    Instruct(APPTERM3): 
#ifdef VERBOSE
 printf("APPTERM3 %d\n", *pc);
#endif
{
	value arg1 = sp[0];
	value arg2 = sp[1];
	value arg3 = sp[2];
	sp = sp + *pc - 3;
	sp[0] = arg1;
	sp[1] = arg2;
	sp[2] = arg3;
	pc = Code_val(accu);
	env = accu;
	extra_args += 2;
	goto check_stacks;
      }

    Instruct(RETURN): 
#ifdef VERBOSE
 printf("RETURN %d\n", *pc);
#endif
{
	sp += *pc++;
	if (extra_args > 0) {
	  extra_args--;
	  pc = Code_val(accu);
	  env = accu;
	} else {
	  pc = (code_t)(sp[0]);
	  env = sp[1];
	  extra_args = Long_val(sp[2]);
	  sp += 3;
	}
	Next;
      }

    Instruct(RESTART): 
#ifdef VERBOSE
 printf("RESTART\n");
#endif
{
	int num_args = Wosize_val(env) - 2;
	int i;
	sp -= num_args;
	for (i = 0; i < num_args; i++) sp[i] = Field(env, i + 2);
	env = Field(env, 1);
	extra_args += num_args;
	Next;
      }

    Instruct(GRAB): 
#ifdef VERBOSE
 printf("GRAB %d\n", *pc);
#endif
{
	int required = *pc++;
	if (extra_args >= required) {
	  extra_args -= required;
	} else {
	  mlsize_t num_args, i;
	  num_args = 1 + extra_args; /* arg1 + extra args */
	  Alloc_small(accu, num_args + 2, Closure_tag);
	  Field(accu, 1) = env;
	  for (i = 0; i < num_args; i++) Field(accu, i + 2) = sp[i];
	  Code_val(accu) = pc - 3; /* Point to the preceding RESTART instr. */
	  sp += num_args;
	  pc = (code_t)(sp[0]);
	  env = sp[1];
	  extra_args = Long_val(sp[2]);
	  sp += 3;
	}
	Next;
      }

    Instruct(CLOSURE): 
#ifdef VERBOSE
 printf("CLOSURE %d %d\n", *pc, *(pc+1));
#endif
{
	int nvars = *pc++;
	int i;
	if (nvars > 0) *--sp = accu;
	/* nvars + 1 <= Max_young_wosize, can allocate in minor heap */
	Alloc_small(accu, 1 + nvars, Closure_tag);
	for (i = 0; i < nvars; i++) Field(accu, i + 1) = sp[i];

	/* The code pointer is not in the heap, so no need to go through
	   caml_initialize. */
	Code_val(accu) = pc + *pc;
	pc++;
	sp += nvars;
	Next;
 }

    Instruct(CLOSUREREC): 
#ifdef VERBOSE
 printf("CLOSURE %d %d %d\n", *pc, *(pc+1), *(pc+2));
#endif
 {
   int nfuncs = *pc++;
   int nvars = *pc++;
   mlsize_t blksize = nfuncs * 2 - 1 + nvars;
   int i;
   value * p;
   if (nvars > 0) *--sp = accu;
	
   Alloc_small(accu, blksize, Closure_tag);
   p = &Field(accu, nfuncs * 2 - 1);
	for (i = 0; i < nvars; i++, p++) *p = sp[i];
	
	sp += nvars;
	/* The code pointers and infix headers are not in the heap,
	   so no need to go through caml_initialize. */
	p = &Field(accu, 0);
	*p = (value) (pc + pc[0]);
	*--sp = accu;
	p++;
	for (i = 1; i < nfuncs; i++) {
	  *p = Make_header(i * 2, Infix_tag, Caml_white);  /* color irrelevant. */
	  p++;
	  *p = (value) (pc + pc[i]);
	  *--sp = (value) p;
	  p++;
	}
	pc += nfuncs;
	Next;
 }

    Instruct(PUSHOFFSETCLOSURE):
#ifdef VERBOSE
 printf("PUSHOFFSETCLOSURE\n");
#endif
      *--sp = accu; /* fallthrough */
    Instruct(OFFSETCLOSURE):
#ifdef VERBOSE
      printf("OFFSETCLOSURE %d\n", *pc);
#endif
      accu = env + *pc++ * sizeof(value); Next;

    Instruct(PUSHOFFSETCLOSUREM2):
#ifdef VERBOSE
      printf("OFFSETCLOSUREM2\n");
#endif
      *--sp = accu; /* fallthrough */
    Instruct(OFFSETCLOSUREM2):
#ifdef VERBOSE
      printf("OFFSETCLOSUREM2\n");
#endif
      accu = env - 2 * sizeof(value); Next;
    Instruct(PUSHOFFSETCLOSURE0):
#ifdef VERBOSE
      printf("PUSH");
#endif
      *--sp = accu; /* fallthrough */
    Instruct(OFFSETCLOSURE0):
#ifdef VERBOSE
      printf("OFFSETCLOSURE0\n");
#endif
      accu = env; Next;
    Instruct(PUSHOFFSETCLOSURE2):
#ifdef VERBOSE
      printf("PUSH");
#endif
      *--sp = accu; /* fallthrough */
    Instruct(OFFSETCLOSURE2):
#ifdef VERBOSE
      printf("OFFSETCLOSURE2\n");
#endif
      accu = env + 2 * sizeof(value); Next;


      /* Access to global variables */

    Instruct(PUSHGETGLOBAL):
#ifdef VERBOSE
      printf("PUSH");
#endif
      *--sp = accu;
      /* Fallthrough */
    Instruct(GETGLOBAL):
#ifdef VERBOSE
      printf("GETGLOBAL %d\n", *pc);
#endif
      accu = Field(caml_global_data, *pc);
      pc++;
      Next;

    Instruct(PUSHGETGLOBALFIELD):
#ifdef VERBOSE
      printf("PUSH");
#endif
      *--sp = accu;
      /* Fallthrough */
    Instruct(GETGLOBALFIELD): 
#ifdef VERBOSE
      printf("GETGLOBALFIELD %d\n", *pc);
#endif
{
	accu = Field(caml_global_data, *pc);
	pc++;
	accu = Field(accu, *pc);
	pc++;
	Next;
      }

    Instruct(SETGLOBAL):
#ifdef VERBOSE
      printf("SETBLOBAL\n", *pc);
#endif
      caml_modify(&Field(caml_global_data, *pc), accu);
      accu = Val_unit;
      pc++;
      Next;

      /* Allocation of blocks */

    Instruct(PUSHATOM0):
#ifdef VERBOSE
      printf("PUSH");
#endif
      *--sp = accu;
      /* Fallthrough */
    Instruct(ATOM0):
#ifdef VERBOSE
      printf("ATOM0\n");
#endif
      accu = Atom(0); Next;

    Instruct(PUSHATOM):
#ifdef VERBOSE
      printf("PUSH");
#endif
      *--sp = accu;
      /* Fallthrough */
    Instruct(ATOM):
#ifdef VERBOSE
      printf("ATOM %d\n", *pc);
#endif
      accu = Atom(*pc++); Next;

    Instruct(MAKEBLOCK): 
#ifdef VERBOSE
      printf("MAKEBLOCK %d %d\n", *pc, *(pc+1));
#endif
{
	mlsize_t wosize = *pc++;
	tag_t tag = *pc++;
	mlsize_t i;
	value block;
	Alloc_small(block, wosize, tag);
	Field(block, 0) = accu;
	for (i = 1; i < wosize; i++) Field(block, i) = *sp++;
	accu = block;
	Next;
      }
    Instruct(MAKEBLOCK1): 
#ifdef VERBOSE
      printf("MAKEBLOCK1 %d\n", *pc);
#endif
{
	tag_t tag = *pc++;
	value block;
	Alloc_small(block, 1, tag);
	Field(block, 0) = accu;
	accu = block;
	Next;
      }
    Instruct(MAKEBLOCK2): 
#ifdef VERBOSE
      printf("MAKEBLOCK2 %d\n", *pc);
#endif
{
	tag_t tag = *pc++;
	value block;
	Alloc_small(block, 2, tag);
	Field(block, 0) = accu;
	Field(block, 1) = sp[0];
	sp += 1;
	accu = block;
	Next;
      }
    Instruct(MAKEBLOCK3): 
#ifdef VERBOSE
      printf("MAKEBLOCK3 %d\n", *pc);
#endif
{
	tag_t tag = *pc++;
	value block;
	Alloc_small(block, 3, tag);
	Field(block, 0) = accu;
	Field(block, 1) = sp[0];
	Field(block, 2) = sp[1];
	sp += 2;
	accu = block;
	Next;
      }
    Instruct(MAKEFLOATBLOCK): 
#ifdef VERBOSE
      printf("MAKEFLOATBLOCK %d\n", *pc);
#endif
{
	mlsize_t size = *pc++;
	mlsize_t i;
	value block;
	Alloc_small(block, size * Double_wosize, Double_array_tag);
	Store_double_field(block, 0, Double_val(accu));
	for (i = 1; i < size; i++){
	  Store_double_field(block, i, Double_val(*sp));
	  ++ sp;
	}
	accu = block;
	Next;
      }

      /* Access to components of blocks */

    Instruct(GETFIELD0):
#ifdef VERBOSE
      printf("GETFIELD0\n");
#endif
      accu = Field(accu, 0); Next;
    Instruct(GETFIELD1):
#ifdef VERBOSE
      printf("GETFIELD1\n");
#endif
      accu = Field(accu, 1); Next;
    Instruct(GETFIELD2):
#ifdef VERBOSE
      printf("GETFIELD2\n");
#endif
      accu = Field(accu, 2); Next;
    Instruct(GETFIELD3):
#ifdef VERBOSE
      printf("GETFIELD3\n");
#endif
      accu = Field(accu, 3); Next;
    Instruct(GETFIELD):
#ifdef VERBOSE
      printf("GETFIELD %d\n", *pc);
#endif
      accu = Field(accu, *pc); pc++; Next;
    Instruct(GETFLOATFIELD): 
#ifdef VERBOSE
      printf("GETFLOATFIELD\n");
#endif
{
	double d = Double_field(accu, *pc);
	Alloc_small(accu, Double_wosize, Double_tag);
	Store_double_val(accu, d);
	pc++;
	Next;
      }

    Instruct(SETFIELD0):
#ifdef VERBOSE
      printf("SETFIELD0\n");
#endif
      caml_modify(&Field(accu, 0), *sp++);
      accu = Val_unit;
      Next;
    Instruct(SETFIELD1):
#ifdef VERBOSE
      printf("SETFIELD1\n");
#endif
      caml_modify(&Field(accu, 1), *sp++);
      accu = Val_unit;
      Next;
    Instruct(SETFIELD2):
#ifdef VERBOSE
      printf("SETFIELD2\n");
#endif
      caml_modify(&Field(accu, 2), *sp++);
      accu = Val_unit;
      Next;
    Instruct(SETFIELD3):
#ifdef VERBOSE
      printf("SETFIELD3\n");
#endif
      caml_modify(&Field(accu, 3), *sp++);
      accu = Val_unit;
      Next;
    Instruct(SETFIELD):
#ifdef VERBOSE
      printf("SETFIELD %d\n", *pc);
#endif
      caml_modify(&Field(accu, *pc), *sp++);
      accu = Val_unit;
      pc++;
      Next;
    Instruct(SETFLOATFIELD):
#ifdef VERBOSE
      printf("SETFLOATFIELD %d\n", *pc);
#endif
      Store_double_field(accu, *pc, Double_val(*sp));
      accu = Val_unit;
      sp++;
      pc++;
      Next;

      /* Array operations */

    Instruct(VECTLENGTH): 
#ifdef VERBOSE
      printf("VECTLENGTH\n");
#endif
{
	mlsize_t size = Wosize_val(accu);
	if (Tag_val(accu) == Double_array_tag) size = size / Double_wosize;
	accu = Val_long(size);
	Next;
      }
    Instruct(GETVECTITEM):
#ifdef VERBOSE
      printf("GETVECTITEM\n");
#endif
      accu = Field(accu, Long_val(sp[0]));
      sp += 1;
      Next;
    Instruct(SETVECTITEM):
#ifdef VERBOSE
      printf("SETVECTITEM\n");
#endif
      caml_modify(&Field(accu, Long_val(sp[0])), sp[1]);
      accu = Val_unit;
      sp += 2;
      Next;

      /* String operations */

    Instruct(GETSTRINGCHAR):
#ifdef VERBOSE
      printf("GETSTRINGCHAR\n");
#endif
      accu = Val_int(Byte_u(accu, Long_val(sp[0])));
      sp += 1;
      Next;
    Instruct(SETSTRINGCHAR):
#ifdef VERBOSE
      printf("SETSTRINGCHAR\n");
#endif
      Byte_u(accu, Long_val(sp[0])) = Int_val(sp[1]);
      sp += 2;
      accu = Val_unit;
      Next;

      /* Branches and conditional branches */

    Instruct(BRANCH):
#ifdef VERBOSE
      printf("BRANCH %d\n", *pc);
#endif
      pc += *pc;
      Next;
    Instruct(BRANCHIF):
#ifdef VERBOSE
      printf("BRANCHIF %d\n", *pc);
#endif
      if (accu != Val_false) pc += *pc; else pc++;
      Next;
    Instruct(BRANCHIFNOT):
#ifdef VERBOSE
      printf("BRANCHIFNOT %d (cond:%d)\n", *pc, accu == Val_false);
#endif
      if (accu == Val_false) pc += *pc; else pc++;
      Next;
    Instruct(SWITCH): 
#ifdef VERBOSE
      printf("SWITCH %d\n", *pc);
#endif
{
	uint32_t sizes = *pc++;
	if (Is_block(accu)) {
	  intnat index = Tag_val(accu);
	  pc += pc[(sizes & 0xFFFF) + index];
	} else {
	  intnat index = Long_val(accu);
	  pc += pc[index];
	}
	Next;
      }
    Instruct(BOOLNOT):
#ifdef VERBOSE
      printf("BOOLNOT\n");
#endif
      accu = Val_not(accu);
      Next;

      /* Exceptions */

    Instruct(PUSHTRAP):
#ifdef VERBOSE
      printf("PUSHTRAP %d\n", *pc);
#endif
      sp -= 4;
      Trap_pc(sp) = pc + *pc;
      Trap_link(sp) = caml_trapsp;
      sp[2] = env;
      sp[3] = Val_long(extra_args);
      caml_trapsp = sp;
      pc++;
      Next;

    Instruct(POPTRAP):
#ifdef VERBOSE
      printf("POPTRAP\n");
#endif
      caml_trapsp = Trap_link(sp);
      sp += 4;
      Next;

    Instruct(RAISE_NOTRACE):
#ifdef VERBOSE
      printf("RAISE_NOTRACE\n");
#endif
      goto raise_notrace;

    Instruct(RERAISE):
#ifdef VERBOSE
      printf("RERAISE\n");
#endif
      goto raise_notrace;

    Instruct(RAISE):
#ifdef VERBOSE
      printf("RAISE\n");
#endif
    raise_exception:
    raise_notrace:
      if ((char *) caml_trapsp
          >= (char *) caml_stack_high - initial_sp_offset) {
        caml_external_raise = initial_external_raise;
        caml_extern_sp = (value *) ((char *) caml_stack_high
                                    - initial_sp_offset);
        return accu;
      }
      sp = caml_trapsp;
      pc = Trap_pc(sp);
      caml_trapsp = Trap_link(sp);
      env = sp[2];
      extra_args = Long_val(sp[3]);
      sp += 4;
      Next;

      /* Stack checks */

    check_stacks:
      if (sp < caml_stack_threshold) {
        caml_extern_sp = sp;
        caml_realloc_stack(Stack_threshold / sizeof(value));
        sp = caml_extern_sp;
      }
      /* Fall through CHECK_SIGNALS */

      /* Signal handling */

    Instruct(CHECK_SIGNALS):    /* accu not preserved */
#ifdef VERBOSE
      printf("CHECK_SIGNALS\n");
#endif
      Next;

      /* Calling C functions */

    Instruct(C_CALL1):
#ifdef VERBOSE
      printf("C_CALL1 %d ", *pc);
      printf(" (* %s *) \n", fun_names[*pc]);
#endif
      Setup_for_c_call;
      accu = Primitive(*pc)(accu);
      Restore_after_c_call;
      pc++;
      Next;
    Instruct(C_CALL2):
#ifdef VERBOSE
      printf("C_CALL2 %d ", *pc);
      printf(" (* %s *) \n", fun_names[*pc]);
#endif
      Setup_for_c_call;
      accu = Primitive(*pc)(accu, sp[1]);
      Restore_after_c_call;
      sp += 1;
      pc++;
      Next;
    Instruct(C_CALL3):
#ifdef VERBOSE
      printf("C_CALL3 %d  ", *pc);
printf(" (* %s *) \n", fun_names[*pc]);		     
#endif
      Setup_for_c_call;
      accu = Primitive(*pc)(accu, sp[1], sp[2]);
      Restore_after_c_call;
      sp += 2;
      pc++;
      Next;
    Instruct(C_CALL4):
#ifdef VERBOSE
      printf("C_CALL4 %d  ", *pc);
printf(" (* %s *) \n", fun_names[*pc]);
#endif
      Setup_for_c_call;
      accu = Primitive(*pc)(accu, sp[1], sp[2], sp[3]);
      Restore_after_c_call;
      sp += 3;
      pc++;
      Next;
    Instruct(C_CALL5):
#ifdef VERBOSE
      printf("C_CALL5 %d  ", *pc);
printf(" (* %s *) \n", fun_names[*pc]);
#endif
      Setup_for_c_call;
      accu = Primitive(*pc)(accu, sp[1], sp[2], sp[3], sp[4]);
      Restore_after_c_call;
      sp += 4;
      pc++;
      Next;
    Instruct(C_CALLN): 
#ifdef VERBOSE
      printf("C_CALLN %d %d ", *pc, *pc);
printf(" (* %s *) \n", fun_names[*pc]);
#endif
{
	int nargs = *pc++;
	*--sp = accu;
	Setup_for_c_call;
	accu = Primitive(*pc)(sp + 1, nargs);
	Restore_after_c_call;
	sp += nargs;
	pc++;
	Next;
      }

      /* Integer constants */

    Instruct(CONST0):
#ifdef VERBOSE
      printf("CONST0\n");
#endif
      accu = Val_int(0); Next;
    Instruct(CONST1):
#ifdef VERBOSE
      printf("CONST1\n");
#endif
      accu = Val_int(1); Next;
    Instruct(CONST2):
#ifdef VERBOSE
      printf("CONST2\n");
#endif
      accu = Val_int(2); Next;
    Instruct(CONST3):
#ifdef VERBOSE
      printf("CONST3\n");
#endif
      accu = Val_int(3); Next;

    Instruct(PUSHCONST0):
#ifdef VERBOSE
      printf("PUSHCONST0\n");
#endif
      *--sp = accu; accu = Val_int(0); Next;
    Instruct(PUSHCONST1):
#ifdef VERBOSE
      printf("PUSHCONST1\n");
#endif
      *--sp = accu; accu = Val_int(1); Next;
    Instruct(PUSHCONST2):
#ifdef VERBOSE
      printf("PUSHCONST2\n");
#endif
      *--sp = accu; accu = Val_int(2); Next;
    Instruct(PUSHCONST3):
#ifdef VERBOSE
      printf("PUSHCONST3\n");
#endif
      *--sp = accu; accu = Val_int(3); Next;

    Instruct(PUSHCONSTINT):
#ifdef VERBOSE
      printf("PUSH");
#endif
      *--sp = accu;
      /* Fallthrough */
    Instruct(CONSTINT):
#ifdef VERBOSE
      printf("CONSTINT %d\n", *pc);
#endif
      accu = Val_int(*pc);
      pc++;
      Next;

      /* Integer arithmetic */

    Instruct(NEGINT):
#ifdef VERBOSE
      printf("NEGINT\n");
#endif
      accu = (value)(2 - (intnat)accu); Next;
    Instruct(ADDINT):
#ifdef VERBOSE
      printf("ADDINT\n");
#endif
      accu = (value)((intnat) accu + (intnat) *sp++ - 1); Next;
    Instruct(SUBINT):
#ifdef VERBOSE
      printf("SUBINT\n");
#endif
      accu = (value)((intnat) accu - (intnat) *sp++ + 1); Next;
    Instruct(MULINT):
#ifdef VERBOSE
      printf("MULINT\n");
#endif
      accu = Val_long(Long_val(accu) * Long_val(*sp++)); Next;

    Instruct(DIVINT):
#ifdef VERBOSE
      printf("DIVINT\n");
#endif
{
	intnat divisor = Long_val(*sp++);
	if (divisor == 0) { Setup_for_c_call; caml_raise_zero_divide(); }
	accu = Val_long(Long_val(accu) / divisor);
	Next;
      }
    Instruct(MODINT): 
#ifdef VERBOSE
      printf("MODINT\n");
#endif
{
	intnat divisor = Long_val(*sp++);
	if (divisor == 0) { Setup_for_c_call; caml_raise_zero_divide(); }
	accu = Val_long(Long_val(accu) % divisor);
	Next;
      }
    Instruct(ANDINT):
#ifdef VERBOSE
      printf("ANDINT\n");
#endif
      accu = (value)((intnat) accu & (intnat) *sp++); Next;
    Instruct(ORINT):
#ifdef VERBOSE
      printf("ORINT\n");
#endif
      accu = (value)((intnat) accu | (intnat) *sp++); Next;
    Instruct(XORINT):
#ifdef VERBOSE
      printf("xORINT\n");
#endif
      accu = (value)(((intnat) accu ^ (intnat) *sp++) | 1); Next;
    Instruct(LSLINT):
#ifdef VERBOSE
      printf("LSLINT\n");
#endif
      accu = (value)((((intnat) accu - 1) << Long_val(*sp++)) + 1); Next;
    Instruct(LSRINT):
#ifdef VERBOSE
      printf("LSRINT\n");
#endif
      accu = (value)((((uintnat) accu - 1) >> Long_val(*sp++)) | 1);
      Next;
    Instruct(ASRINT):
#ifdef VERBOSE
      printf("ASRINT\n");
#endif
      accu = (value)((((intnat) accu - 1) >> Long_val(*sp++)) | 1); Next;

#define Integer_comparison(typ,opname,tst)			\
      Instruct(opname):						\
	accu = Val_int((typ) accu tst (typ) *sp++); Next;

      Integer_comparison(intnat,EQ, ==)
	Integer_comparison(intnat,NEQ, !=)
	Integer_comparison(intnat,LTINT, <)
	Integer_comparison(intnat,LEINT, <=)
	Integer_comparison(intnat,GTINT, >)
	Integer_comparison(intnat,GEINT, >=)
	Integer_comparison(uintnat,ULTINT, <)
	Integer_comparison(uintnat,UGEINT, >=)

#define Integer_branch_comparison(typ,opname,tst,debug) \
	Instruct(opname):				\
	  if ( *pc++ tst (typ) Long_val(accu)) {	\
	    pc += *pc ;					\
	  } else {					\
	    pc++ ;					\
	  } ; Next;

	Integer_branch_comparison(intnat,BEQ, ==, "==")
	Integer_branch_comparison(intnat,BNEQ, !=, "!=")
	Integer_branch_comparison(intnat,BLTINT, <, "<")
	Integer_branch_comparison(intnat,BLEINT, <=, "<=")
	Integer_branch_comparison(intnat,BGTINT, >, ">")
	Integer_branch_comparison(intnat,BGEINT, >=, ">=")
	Integer_branch_comparison(uintnat,BULTINT, <, "<")
	Integer_branch_comparison(uintnat,BUGEINT, >=, ">=")

	Instruct(OFFSETINT):
#ifdef VERBOSE
		       printf("OFFSETINT %d\n", *pc);
#endif
      accu += *pc << 1;
      pc++;
      Next;

    Instruct(OFFSETREF):
#ifdef VERBOSE
		       printf("OFFSETREF %d\n", *pc);
#endif
      Field(accu, 0) += *pc << 1;
      accu = Val_unit;
      pc++;
      Next;
    Instruct(ISINT):
#ifdef VERBOSE
      printf("ISINT\n");
#endif
      accu = Val_long(accu & 1);
      Next;

      /* Object-oriented operations */

#define Lookup(obj, lab) Field (Field (obj, 0), Int_val(lab))

      /* please don't forget to keep below code in sync with the
         functions caml_cache_public_method and
         caml_cache_public_method2 in obj.c */

    Instruct(GETMETHOD):
#ifdef VERBOSE
      printf("GETMETHOD\n");
#endif
      accu = Lookup(sp[0], accu);
      Next;

#define CAML_METHOD_CACHE
#ifdef CAML_METHOD_CACHE
    Instruct(GETPUBMET): 
#ifdef VERBOSE
		       printf("GETPUBMET %d %d\n", *pc, *(pc+1));
#endif
{
	/* accu == object, pc[0] == tag, pc[1] == cache */
	value meths = Field (accu, 0);
	value ofs;
	*--sp = accu;
	accu = Val_int(*pc++);
	ofs = *pc & Field(meths,1);
	if (*(value*)(((char*)&Field(meths,3)) + ofs) == accu) {
	  accu = *(value*)(((char*)&Field(meths,2)) + ofs);
	}
	else
	  {
	    int li = 3, hi = Field(meths,0), mi;
	    while (li < hi) {
	      mi = ((li+hi) >> 1) | 1;
	      if (accu < Field(meths,mi)) hi = mi-2;
	      else li = mi;
	    }
	    *pc = (li-3)*sizeof(value);
	    accu = Field (meths, li-1);
	  }
	pc++;
	Next;
      }
#else
    Instruct(GETPUBMET):
#ifdef VERBOSE
		       printf("GETPUBMET %d %d\n", *pc, *(pc+1));
#endif
      *--sp = accu;
      accu = Val_int(*pc);
      pc += 2;
      /* Fallthrough */
#endif
    Instruct(GETDYNMET): 
#ifdef VERBOSE
		       printf("GETDYNMET\n");
#endif
{
	/* accu == tag, sp[0] == object, *pc == cache */
	value meths = Field (sp[0], 0);
	int li = 3, hi = Field(meths,0), mi;
	while (li < hi) {
	  mi = ((li+hi) >> 1) | 1;
	  if (accu < Field(meths,mi)) hi = mi-2;
	  else li = mi;
	}
	accu = Field (meths, li-1);
	Next;
      }

      /* Debugging and machine control */

    Instruct(STOP):
#ifdef VERBOSE
		  printf("STOP\n");
#endif
      caml_external_raise = initial_external_raise;
      caml_extern_sp = sp;
      return accu;

    Instruct(EVENT):
#ifdef VERBOSE
		  printf("EVENT\n");
#endif
      Next;

    Instruct(BREAK):
#ifdef VERBOSE
		  printf("BREAK\n");
#endif
      Next;

    default:
#if _MSC_VER >= 1200
      __assume(0);
#else
      caml_fatal_error_arg("Fatal error: bad opcode (%"
                           ARCH_INTNAT_PRINTF_FORMAT "x)\n",
                           (char *) (intnat) *(pc-1));
#endif
    }
  }
}
