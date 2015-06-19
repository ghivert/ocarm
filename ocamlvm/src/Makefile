CC = clang

SRC = interp.c parse.c
OBJ = $(SRC:%.c=%.o)

CFLAGS = -std=gnu11 -Wall -Wextra

all: ocamlvm

ocamlvm: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJ) ocamlvm
