.SUFFIXES: .c .l .o .so .y .yy.c .yy.o

CFLAGS?=-O2
CFLAGS+= -Wall -Wpedantic -Werror -Wformat=2 -Wshadow -Wpointer-arith \
	-Wcast-qual -Wstrict-aliasing=2 -Wwrite-strings -Wstack-protector
CFLAGS+= ${CFLAGS_$@}

.c.o:
	${CC} -fPIC ${CFLAGS} -c -o $@ $<

.l.yy.c:
	${LEX} ${LFLAGS} -o $@ $<

.o.so:
	${CC} -shared ${CFLAGS} -o $@ $>

.y.c:
	${YACC} -d ${YFLAGS} -o $@ $<

.yy.c.yy.o:
	${CC} -fPIC ${CFLAGS} -c -o $@ $<
