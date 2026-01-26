#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * pipe_factoring.c
 *
 * Task:
 * =====
 *  Here is some code that uses fork to speed up factoring, though does facting
 *  in a super dumb way.
 *
 * Step 1:
 * =======
 *  Read and understand the code, answer the following questions.
 *
 *  1. How many processes will be created?
 *  2. What is the job of each process?
 *
 *  Hint: Label each area of code with the name of the process(es) that would be
 *  executing there.
 *
 * Step 2:
 * =======
 *  Modify this code so that rather than have the 2 child processes individualy
 *  print out the factors discovered, instead each child process uses IPC to
 *  communicate to the parent process its results.
 *  The parent process would be the one to print out all the factors.
 *
 *  IT IS OKAY IF DOING THIS CHANGES THE ORDER IN WHICH FACTORS SHOW UP.
 *  You should be okay as long as all factors are printed BY THE PARENT.
 */

int main(void)
{
  unsigned long long int target, i, start = 0;
  printf("Give a number to factor.\n");
  // 18446744073709551615 is max, I think
  // I think 666666662 gives good results
  scanf("%llu",&target);

  int pid = fork();
  if(pid == 0) {
    start = 2;
  } else {
    pid = fork();
    if(pid == 0) {
      start = 3;
    } else {
      // I'm the parent
      wait(NULL);
      wait(NULL);
      printf("Finished.\n");
      return 0;
    }
  }

  for(i = start; i <= target/2; i = i + 2) {
    if(target % i == 0) {
      printf("%llu is a factor\n", i);
    }
  }
  return 0;
}
