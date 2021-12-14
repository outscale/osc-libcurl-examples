all: example01 example02 example03

debug: example01debug example02debug example03debug

example01:
	gcc example01.c -o example01 $(CFLAGS) -lcurl $(LDFLAGS)

example01debug:
	gcc example01.c -o example01debug $(CFLAGS) -g -lcurl $(LDFLAGS)

example02:
	gcc example02.c -o example02 $(CFLAGS) -lcurl $(LDFLAGS)

example02debug:
	gcc example02.c -o example02debug $(CFLAGS) -g -lcurl $(LDFLAGS)

example03:
	gcc example03.c -o example03 $(CFLAGS) -lcurl $(LDFLAGS)

example03debug:
	gcc example03.c -o example03debug $(CFLAGS) -g -lcurl $(LDFLAGS)

clean:
	rm -rvf example01 example02 example03
	rm -rvf example01debug example02debug example03debug

clean_dsym: clean
	rm -rvf example01.dSYM example02.dSYM example03.dSYM

.PHONY: clean clean_dsym
