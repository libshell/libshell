.SUFFIXES: .c .l .o .y .yy.c .yy.o
.PHONY: all clean

CFLAGS?=-O2
CFLAGS+= -Wall -Wpedantic -Werror -Wformat=2 -Wshadow -Wpointer-arith \
	-Wcast-qual -Wstrict-aliasing=2 -Wwrite-strings -Wstack-protector
CFLAGS+= ${CFLAGS_$@}

all: sh.so
sh.so: sh.yy.o sh.o
	${CC} -shared ${CFLAGS} -o $@ $>

sh.o: ../libshell.h
CFLAGS_sh.yy.o=-Wno-keyword-macro -Wno-unused-function -Wno-unneeded-internal-declaration
sh.yy.o: sh.h sh.c
sh.h: sh.c

.yy.c.yy.o:
	${CC} -fPIC ${CFLAGS} -c -o $@ $<

.c.o:
	${CC} -fPIC ${CFLAGS} -c -o $@ $<

.l.yy.c:
	${LEX} ${LFLAGS} -o $@ $<

.y.c:
	${YACC} -d ${YFLAGS} -o $@ $<

clean:
	rm -f *.o *.so *.yy.c sh.c sh.h
