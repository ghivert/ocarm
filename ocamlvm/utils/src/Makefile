.PHONY: clean

../bin/bin2c: bin2c.c
	mkdir -p ../bin
	clang -o $@ $<

clean:
	$(RM) ../bin
