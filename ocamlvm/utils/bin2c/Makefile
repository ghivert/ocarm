.PHONY: clean

bin/bc2c: src/bc2c.c
	mkdir -p bin
	clang -o $@ $<

clean:
	$(RM) -r bin
