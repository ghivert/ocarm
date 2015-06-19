.PHONY: clean

../bin/bc2c: bc2c.c
	mkdir -p ../bin
	clang -o $@ $<

clean:
	$(RM) -r ../bin
