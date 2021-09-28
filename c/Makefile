

all: example01 example02 example03 test

CFLAGS+= "-g"

example01:
	gcc example01.c -o example01 $(CFLAGS) -lcurl $(LDFLAGS)

example02:
	gcc example02.c -o example02 $(CFLAGS) -lcurl $(LDFLAGS)

example03:
	gcc -g example03.c -o example03 $(CFLAGS) -lcurl $(LDFLAGS)

clean:
	rm -rvf example01 example02 example03

.PHONY: test
