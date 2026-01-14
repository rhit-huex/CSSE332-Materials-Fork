#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
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
    // child 1
    while(1)
      ;
  } else {
    rc = fork();
    if(rc < 0) {
      perror("fork failed");
      exit(EXIT_FAILURE);
    }

    if(rc == 0) {
      // child 2
      printf("hello from child 2\n");
      exit(EXIT_FAILURE);
    }
  }

  wait(0);

  return 0;
}
