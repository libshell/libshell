.include <./Makefile.inc>

.PHONY: all backend clean frontend

DIRS = backend frontend

all: libshell ${DIRS}

${DIRS}: common.o
	cd $@ && ${MAKE}

libshell: main.o common.o
	${CC} -Wl,--export-dynamic ${CFLAGS} -o $@ $>

CFLAGS_main.o=-Wno-pedantic
main.o: libshell.h

clean:
	rm -f *.o libshell
	for dir in ${DIRS}; do (cd $$dir && ${MAKE} clean); done
