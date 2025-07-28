PHONY: mystrtest dividint factorial

mystrtest: mystring.c mystring.h mystrtest.c
	gcc -g mystring.c mystrtest.c -o mystrtest

dividint: ./dividint.c
	gcc ./dividint.c -g -o dividint

factorial: factorial.c
	gcc ./factorial.c -g -o factorial

clean:
	rm -rf mystrtest dividint factorial *.o *.dSYM
