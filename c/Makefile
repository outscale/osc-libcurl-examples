

all: example01 example02 example03 test

CFLAGS+= "-g"

example01:
	gcc example01.c -o example01 -lcurl $(CFLAGS)

example02:
	gcc example02.c -o example02 -lcurl $(CFLAGS)

example03:
	gcc -g example03.c -o example03 -lcurl $(CFLAGS)

clean:
	rm -rvf example01 example02 example03

.PHONY: test
