#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
  int rc;

  pid_t mypid = getpid();

  rc = fork();
  if(rc < 0) {
    perror("fork failed:");
    exit(EXIT_FAILURE);
  }

  if(rc == 0) {
    // child
    printf("Hello from the child process, my process id is %d!\n", mypid);
    exit(EXIT_SUCCESS);
  }

  // parent
  printf("Hello from the parent, my process id is %d!\n", mypid);
  sleep(1);

  return 0;
}
