CC = clang

SRC = alloc.c    compare.c  extern.c    floats.c  
      heap_pp.c  ints.c     md5.c       obj.c    
      array.c    custom.c   fail.c      gc.c      
      intern.c   main.c     memory.c    stacks.c  
      str.c      dynlink.c  fix_code.c  hash.c   
      interp.c   misc.c     startup.c

OBJ = $(SRC:%.c=%.o)

CFLAGS = -std=c99 -Wall -Wextra

all: ocamlvm

ocamlvm: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJ) ocamlvm
	$(RM) *.cmo *.cmi
