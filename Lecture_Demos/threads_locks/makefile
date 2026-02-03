all: simple_sum.run array_sum.run

simple_sum.run: simple_sum.c
	gcc -ggdb -pthread simple_sum.c -o simple_sum.run

array_sum.run: array_sum.c
	gcc -ggdb -pthread array_sum.c -o array_sum.run

clean:
	rm -rf *.run *.o *.dSYM

