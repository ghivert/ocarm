SRC=src/modinc.c src/strings.c

all: modinc

modinc: $(SRC)
	mkdir -p bin
	clang -o $@ $(SRC) -Iinc

clean:
	$(RM) -r bin modinc
