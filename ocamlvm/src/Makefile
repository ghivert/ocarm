CC = clang

SRC = main.c startup.c interp.c fix_code.c gc.c misc.c intern.c
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
