#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
  int rc;

  rc = fork();
  if(rc < 0) {
    perror("fork failed:");
    exit(EXIT_FAILURE);
  }

  if(rc == 0) {
    // child
    printf("Hello from the child process!\n");
    exit(EXIT_SUCCESS);
  }

  // parent
  printf("Hello from the parent, I created child %d!\n", rc);
  sleep(1);

  return 0;
}
