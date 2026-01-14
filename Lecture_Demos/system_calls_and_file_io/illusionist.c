/*
 * CSSE 332: Operating Systems.
 *
 * This file allocates space for a signle integer and
 * then changes its value.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>

int
main(int argc, char *argv[])
{
	int *iptr;
  int i = 0;

  srand(time(NULL));

	/* let's allocate and print an integer */
	iptr = malloc(sizeof(int));
	if (!iptr) {
		perror("Ooops, out of memory...\n");
		exit(EXIT_FAILURE);
	}
	*iptr = rand();

	printf("(%u): Allocated int at location %p\n", getpid(), iptr);

	while(i < 10) {
		*iptr = (*iptr) + 1;
		printf("(%u): Updated the pointer's value to %d\n", getpid(),
		       *iptr);
		sleep(1);
    i++;
	}

	/* done */
	free(iptr);
}
