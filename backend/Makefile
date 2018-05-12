.SUFFIXES: .c .o .so
.PHONY: all clean

CFLAGS?=-O2
CFLAGS+= -Wall -Wpedantic -Werror -Wformat=2 -Wshadow -Wpointer-arith \
	-Wcast-qual -Wstrict-aliasing=2 -Wwrite-strings -Wstack-protector
CFLAGS+= ${CFLAGS_$@}

all: exec.so

exec.o: ../libshell.h exec.h

.o.so:
	${CC} -shared ${CFLAGS} -o $@ $>

.c.o:
	${CC} -fPIC ${CFLAGS} -c -o $@ $<

clean:
	rm -f *.o *.so
