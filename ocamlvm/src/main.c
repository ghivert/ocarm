#include "startup.c"
#include <stdlib.h>

int main (int argc, char* argv[]) {
  caml_main(argv);
  return 0;
}