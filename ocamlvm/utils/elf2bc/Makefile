CC = clang

CGLAGS = -std=gnu11 -Wall -Wextra

elf2bc : bin/elf2bc.o 
	$(CC) $(CFLAGS) -o $@ $^

bin/%.o: src/%.c
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	$(RM) -r elf2bc bin
