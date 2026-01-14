#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int pid = fork();

	if (pid < 0) {
		perror("Fork failed.\n");
		return 1;  // nonzero means a failure result in unix
	}

	if (pid == 0) {
    printf("Child (%d): Hello from child process\n", getpid());
    printf("Child (%d): I am done playing, goodbye...\n", getpid());
    exit(0);
	}

  printf("Parent (%d): Going into infinite loop...\n", getpid());
  while(1);

  // dead code
	return 0;
}
