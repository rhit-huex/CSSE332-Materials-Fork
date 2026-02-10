/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * @author <Your name>
 * @date   <Date last modified>
 */
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Here is some code that factors in a super dumb way.  We won't be
// attempting to improve the algorithm in this case (though that would be
// the correct thing to do).
//
// Modify the code so that it starts the specified number of threads and
// splits the computation among them.  You can be sure the max allowed
// number of threads is 50.  Be sure your threads actually run in parallel.
//
// Your threads should each just print the factors they find, they don't
// need to communicate the factors to the original thread.

int
main(void)
{
  unsigned long long int target, i;
  int numThreads;

  printf("Give a number to factor.\n");
  scanf("%llu", &target);

  printf("How man threads should I create?\n");
  scanf("%d", &numThreads);
  if(numThreads > 50 || numThreads < 1) {
    printf("Bad number of threads!\n");
    return 0;
  }

  for(i = 2; i <= target / 2; i = i + 1) {
    // You'll want to keep this testing line in.  Otherwise it goes so fast it
    // can be hard to detect your code is running in parallel. Also test with a
    // large number (i.e. > 3000)
    printf("testing %llu\n", i);
    if(target % i == 0) {
      printf("%llu is a factor\n", i);
    }
  }
  return 0;
}
