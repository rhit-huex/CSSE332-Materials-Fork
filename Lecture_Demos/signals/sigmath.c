#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * sigmath.c
 *
 * This activity is broken down in two steps:
 *
 * Step 1:
 *  Read the code below, predict what will be printed on the console.
 *
 * Step 2:
 *  Compile the code using `make` and then run it using `./sigmath.run`.
 *  Did the output match your expectation?
 *
 * Step 3:
 *  Fix the code so that we print a custom message before we exit the process.
 *
 *  Hint: Use `man signal` to see what type of signals to work with.
 *
 */

int
main(int argc, char **argv)
{
  unsigned int x = -1;
  unsigned int y = 30;

  x = x + 1;
  y = y / x;

  printf("After complex math, y = %u\n", y);
  return 0;
}
