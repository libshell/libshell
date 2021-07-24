.SUFFIXES: .c .l .o .so .y .yy.c .yy.o

CFLAGS?=-O2
WERROR ?= -Werror
CFLAGS += \
	$(WERROR) \
	-Wall \
	-Wcast-qual \
	-Wextra \
	-Wformat=2 \
	-Wpedantic \
	-Wpointer-arith \
	-Wshadow \
	-Wsign-compare \
	-Wsign-conversion \
	-Wstack-protector \
	-Wstrict-aliasing=2 \
	-Wwrite-strings \
	-fno-common
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
