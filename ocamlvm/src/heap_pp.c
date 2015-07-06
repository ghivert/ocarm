#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[1;4;31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void heap_pp(char *heap, int heap_size, char *name, int nth) {
  if (nth > heap_size) {
    fprintf(stderr, "Error in your nth number.\n");
    return;
  }
  int cpt = 0; int max = 0;
  char buffer[1000];
  
  for (int i = 0; i < heap_size; i++) {
    sprintf(buffer, "%d", heap[i]);
    max = (((int) strlen(buffer)) > max) ? strlen(buffer) : max;
  }
  
  printf(ANSI_COLOR_RED "%s :" ANSI_COLOR_RESET "\n", name);
  for (int i = 0; i < nth; i++) {
    printf("**" ANSI_COLOR_GREEN "   %d   " ANSI_COLOR_RESET, heap[i]);
    sprintf(buffer, "%d", heap[i]);
    if (((int) strlen(buffer)) < max)
      for (int j = 0; j < (int) (max - strlen(buffer)); j++)
	printf(" ");
    if (++cpt > 8) {
      printf("**\n");
      cpt = 0;
    }
  }
  
  printf("**\n");
}
