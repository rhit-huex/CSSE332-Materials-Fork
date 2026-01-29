#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // Read #of children from command line: argv[1]
  int numChildren = atoi(argv[1]);
  printf("Test numChildren: %d \n", numChildren);
  fflush(stdout);
  // First process
  printf("Parent->");
  fflush(stdout);
  for(int i=0; i<numChildren; i++) {
    int rc = fork();
    if(rc == 0) { // child
      if(i == (numChildren-1)) { printf("child(%d)", i); fflush(stdout); }
      else { printf("child(%d)->", i); fflush(stdout); }
    } else { // parent
      wait(0);
      exit(0);
    }
  }
  printf("\n");
}
