srcs=$(wildcard *.c)
targets=$(srcs:%.c=%.run)

.PHONY: clean all

all: $(targets)

%.run: %.o
	$(CC) $(CFLAGS) -o $@ $^ -pthread

clean:
	rm -rf *.o *.run
