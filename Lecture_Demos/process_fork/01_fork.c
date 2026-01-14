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

  printf("Hello World!\n");
  sleep(1);

  return 0;
}
