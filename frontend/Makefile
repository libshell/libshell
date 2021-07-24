.include <../Makefile.inc>

.PHONY: all clean

all: sh.so
sh.so: sh.yy.o sh.o
	${CC} -shared ${CFLAGS} -o $@ $>

sh.o: ../libshell.h
CFLAGS_sh.yy.o=-Wno-keyword-macro -Wno-unused-function -Wno-unneeded-internal-declaration -Wno-sign-conversion
sh.yy.o: sh.h sh.c
sh.h: sh.c

clean:
	rm -f *.o *.so *.yy.c sh.c sh.h
