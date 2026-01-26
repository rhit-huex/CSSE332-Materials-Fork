SRC = $(wildcard *c)
BINS = $(patsubst %.c, %.run, $(SRC))

CFLAGS += -ggdb -Werror

.PHONY: all clean

all: ${BINS}

%.run: %.o
	${CC} ${CFLAGS} ${LDLIBS} -o $@ $^

clean:
	rm -rf ${BINS} *.o
