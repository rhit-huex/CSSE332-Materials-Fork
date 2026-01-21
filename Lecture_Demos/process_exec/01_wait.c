#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_CHILDREN 3

// TODO
// ====
//  As an exercise, modify this code so that we wait for the children in the
//  reverse order in which we create them!

int
main()
{
  int pid;
  int status;
  int rc;

  for(int i = 0; i < NUM_CHILDREN; i++) {
    pid = fork();
    if(pid < 0) {
      perror("Fork failed.\n");
      return 1; // nonzero means a failure result in unix
    }

    if(pid == 0) {
      // child
      srand(getpid());
      printf("I am child %d and my PID is %d\n", i + 1, getpid());
      sleep(1);

      rc = rand() % 10;
      printf("Child (%d): Done and will exit with value %d!\n", i + 1, rc);
      if(rc >= 5) {
        // crash
        int *p = 0x00;
        *p     = 3;
      }
      exit(rc); // alternatively, can do return rc;
    }

    // parent
    printf("Parent: I created a child with PID: %d\n", pid);
  }

  for(int i = 0; i < NUM_CHILDREN; i++) {
    rc = wait(&status);
    if(rc < 0) {
      fprintf(stderr, "Parent: Something bad happened to my child!\n");
      exit(EXIT_FAILURE);
    }

    if(WIFEXITED(status)) {
      // child exited
      printf("Parent: Child (%d) returned with exit code %d\n", rc,
             WEXITSTATUS(status));
    } else {
      printf("Parent: Child (%d) crashed :(\n", rc);
    }
  }

  return 0;
}
