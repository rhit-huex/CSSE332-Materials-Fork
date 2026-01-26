#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

/**
 * pipe_rand.c
 *
 * Task:
 * =====
 *  In this exercise, we would like the child to send a random number of
 *  characters to its parent.
 *
 *  The trick is that we don't know beforehand what the random number would be,
 *  so we cannot know how many characters the child will be sending to its
 *  parent.
 *  This means we need a way for the parent to continue reading until the child
 *  has done sending information.
 *
 * Hint:
 * ====
 *  Read the man page for read. Check out its return value, it would be very
 *  useful in this task.
 *
 */

char get_rand_char(void) {
  return (rand() % 35) + 65;
}

/**
 * child_fn()
 *
 * The child function.
 *
 * @param write_fd  The file descriptor for the writing end of a pipe.
 * @return nothing.
 */
void child_fn(int write_fd) {
  printf("[Child (%d)] Writer started...\n", getpid());
}

/**
 * parent_fn()
 *
 * The parent function.
 *
 * @param read_fd   The file descriptor for the reading end of a pipe.
 * @return nothing.
 */
void parent_fn(int read_fd) {
  printf("[Parent (%d)] Reader started...\n", getpid());
}

int main(int argc, char **argv) {
  // TODO: Declare your variables here.

  // seed the random number generator
  srand(time(0));

  // TODO: Set up your forks here.
  //        Have the child call the child_fn function.
  //        Have the parent call the parent_fn function.
  //
  // HINT: use the get_rand_char function to get a random character.
  // HINT: use `int x = rand() % 40;` to get a random integer between 0 and 40.
  
  exit(0);
}

