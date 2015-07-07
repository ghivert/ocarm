CC = clang

SRC = alloc.c      custom.c    fix_code.o   heap_pp.c      \
      io.c         memory.c    printexc.c     startup.o    \
      array.c      debugger.c  floats.c     instrtrace.c   \
      lexing.c     meta.c      roots.c        str.c        \
      backtrace.c  dynlink.c   freelist.c   intern.c       \
      main.c       minor_gc.c  signals_byt.c  sys.c        \
      callback.c   extern.c    gc.c         intern.o       \
      main.o       misc.c      signals.c      terminfo.c   \
      caml         fail.c      gc.o         interp.c       \
      major_gc.c   misc.o      stacks.c       unix.c       \
      compact.c    finalise.c  globroots.c  interp.o       \
      Makefile     obj.c       startup_aux.c  weak.c       \
      compare.c    fix_code.c  hash.c       ints.c         \
      md5.c        parsing.c   startup.c      win32.c

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
