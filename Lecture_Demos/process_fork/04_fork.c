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
    printf("Hello from the child process, my process id is %d!\n", getpid());
    while(1)
      ;
    exit(EXIT_SUCCESS);
  }

  // parent
  printf("Hello from the parent, my process id is %d!\n", getpid());
  sleep(1);

  return 0;
}
