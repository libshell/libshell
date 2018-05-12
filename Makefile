.SUFFIXES: .c .o
.PHONY: all backend clean frontend

CFLAGS?=-O2
CFLAGS+= -Wall -Wpedantic -Werror -Wformat=2 -Wshadow -Wpointer-arith \
	-Wcast-qual -Wstrict-aliasing=2 -Wwrite-strings -Wstack-protector
CFLAGS+= ${CFLAGS_$@}

DIRS = backend frontend

all: libshell ${DIRS}

${DIRS}: common.o
	cd $@ && ${MAKE}

libshell: main.o common.o
	${CC} -Wl,--export-dynamic ${CFLAGS} -o $@ $>

CFLAGS_main.o=-Wno-pedantic
main.o: libshell.h

.c.o:
	${CC} -fPIC ${CFLAGS} -c -o $@ $<

clean:
	rm -f *.o libshell
	for dir in ${DIRS}; do (cd $$dir && ${MAKE} clean); done
