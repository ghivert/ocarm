#include "stdlib.h"
#include "stdio.h"
#include "instruct.h"

#define HEAP_SIZE 200
#define STACK_SIZE 100

int interp (int* prog) {
  
  register int* pc = prog;
  register int* sp = malloc(sizeof(int) * STACK_SIZE); 
  int *sp_init = sp + STACK_SIZE - 1;
  for (int i = 0; i < STACK_SIZE; i++)
    sp[i] = -1;
  sp += STACK_SIZE;
  register int acc;
  int* heap = malloc(sizeof(int) * HEAP_SIZE); int heap_ind = 0;
  for (int i = 0; i < HEAP_SIZE; i++)
    heap[i] = -1;

  int start; int tag;
  while (1) {

    printf("Stack : ");
    for (int i = 0; i < STACK_SIZE / 10; i++)
      printf("%d - ", sp_init[-i]);
    printf("\nHeap : ");
    for (int i = 0; i < HEAP_SIZE / 20; i++)
      printf("%d - ", heap[i]);
    printf("\nHeap pointer : %d\nAcc : %d\n*************    ", heap_ind, acc);

    switch (*pc++) {
    case ACC0:
      acc = *sp;
      printf("ACC0\n");
      break;
    case PUSHACC0 :
      *--sp = acc;
      printf("PUSHACC0\n");
      break;
    case PUSHACC2 :
      *--sp = acc;
      acc = sp[2];
      printf("PUSHACC2\n");
      break;
    case PUSHACC4 :
      *--sp = acc;
      acc = sp[4];
      printf("PUSHACC4\n");
      break;
    case POP:
      sp += *pc++;
      printf("POP %d\n", *(pc-1));
      break;
    case CLOSURE:
      int nvars = *pc++;
      printf("CLOSURE %d %d\n", *(pc-1), *pc);
      if (nvars > 0) *--sp = accu;
      start = heap_ind;
      heap[heap_ind++] = ((1 + nvars) << 10 | (0 << 8 ) | 247); // 247 : closure tag
      heap[heap_ind++] = pc + *pc; 
      pc++;
      for (int i = 0; i < nvars; i++)
	heap[heap_ind++] = *(sp + i);
      sp += nvars;
      break;
    case SETGLOBAL:
      printf("SETGLOBAL\n");
      return 0;
      break; 
    case MAKEBLOCK1:
      tag = *pc++;
      start = heap_ind;
      heap[heap_ind++] = (1 << 10 | (0 << 8) | tag);
      heap[heap_ind++] = acc;
      acc = start;
      printf("MAKEBLOCK1 %d\n", *(pc - 1));
      break;
    case MAKEBLOCK2:
      tag = *pc++;
      start = heap_ind;
      heap[heap_ind++] = (2 << 10 | (0 << 8) | tag);
      heap[heap_ind++] = acc;
      heap[heap_ind++] = *sp++;
      acc = start;
      printf("MAKEBLOCK2 %d\n", *(pc - 1));
      break;
    case MAKEBLOCK3:
      tag = *pc++;
      start = heap_ind;
      heap[heap_ind++] = (3 << 10 | (0 << 8) | tag);
      heap[heap_ind++] = acc;
      heap[heap_ind++] = *sp++;
      heap[heap_ind++] = *sp++;
      acc = start;
      printf("MAKEBLOCK3 %d\n", *(pc - 1));
      break;
    case BRANCH:
      printf ("CONSTINT %d\n", *pc);
      pc += *pc;
      break;
    case CONSTINT :
      acc = *pc++;
      printf ("CONSTINT %d\n", acc);
      break;
    case PUSHCONSTINT :
      *--sp = acc;
      printf ("PUSHCONSTINT %d\n", *pc);
      acc = *pc++;
      break;
    case OFFSETINT :
      acc += *pc++;
      printf ("OFFSETINT %d\n", *(pc -1));
      // light_led_number(acc);
      break;
    case ADDINT:
      acc += *sp++;
      printf ("ADDINT\n");
      // light_led_number(acc);
      break;
    default:
      printf ("OPCODE inconnu : %d\n", *(pc - 1));
      break;
    }
  }
}

int main() {
  int prog[] = { 103, 10, 108, 15, 
		 10,  12, 110, 10, 
		 127, 17, 10,  0, 
		 12,  14, 65,  0, 
		 19,  3,  57,  0 };
  interp(prog);
  return 0;
}

