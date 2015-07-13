CC = clang

SRC = 	alloc.c     custom.c    heap_pp.c    memory.c      \
	array.c     floats.c    str.c        md5.c         \
	intern.c    main.c      startup.c    gc.c          \
	misc.c	    fail.c      interp.c     stacks.c      \
	obj.c       compare.c   fix_code.c   hash.c        \
	ints.c          	     

OBJ = $(SRC:%.c=%.o)

CFLAGS = -std=c99 -Wall -Wextra

all: ocamlvm

ocamlvm: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJ) ocamlvm
	$(RM) *.cmo *.cmi
