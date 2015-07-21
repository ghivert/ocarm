CC = clang

SRC = getBytecode.c

OBJ = $(SRC:%.c=%.o)

CGLAGS = -std=c99 -Wall -Wextra

getBytecode : $(OBJ) 
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(OBJ) getBytecode