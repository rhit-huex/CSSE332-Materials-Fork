#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

//int globalvar = 10;

void main() {
  int* globalvar = malloc(sizeof(int));
  *globalvar = 10; // allocating on heap

  int rc = fork();

  if(rc == 0) {
    // child
    *globalvar = 255; // modify the globalvar
    printf("Child ran: %d\n", *globalvar);
    exit(7);
  } else {
    // parent
  }

  int wstatus;
  // wait(&wstatus);
  while(1);
  // this causes a zombie since not waiting means that we are not releasing the
  // exit status that was generated in memory on a call to exit
  // The purpose of the while loop is to keep the parent alive to see this because
  // modern linux, killing the parent will kill its children
  printf("Parent's global var: %d\n", *globalvar);


  if(WIFEXITED(wstatus)) {
    int statusCode = WEXITSTATUS(wstatus);
    printf("Return code from child: %d\n", statusCode);
  }

  printf("Who hits this line? %d \n", getpid());
}
