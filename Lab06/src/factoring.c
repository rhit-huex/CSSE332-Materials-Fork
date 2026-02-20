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
int numThreads = 0;
unsigned long long int target = 0;

void* factor(void* threadId) { // target and numThreads are global and we dont change them
  // Break down target/2 into sections by dividing by numThreads
  // then start is the threadID*section and end is threadID*section + section
  int start = ((target/2) / numThreads) * (*(int*)threadId);
  int end = start + ((target/2) / numThreads);
  //fprintf(stdout, "Thread: %d\nStart: %d\nEnd: %d\n", *(int*)threadId, start, end);
  //fprintf(stdout, "Thread: %d\n", *(int*)threadId);
  for(int i = start; i < end; i = i + 1) {
    // You'll want to keep this testing line in.  Otherwise it goes so fast it
    // can be hard to detect your code is running in parallel. Also test with a
    // large number (i.e. > 3000)
    printf("testing %d\n", i);
    if(i == 0) {
      printf("0 is not a factor\n");
    }
    if(i!=0 && target % i == 0) {
      printf("%d is a factor\n", i);
    }
  }
  if(target/2 == end) {
    if(target % end == 0) {
      printf("%d is a factor\n", end);
    }
  }
  return NULL;
}

int main(void) {
  printf("Give a number to factor.\n");
  scanf("%llu", &target);

  printf("How man threads should I create?\n");
  scanf("%d", &numThreads);
  if(numThreads > 50 || numThreads < 1) {
    printf("Bad number of threads!\n");
    return 0;
  }
  pthread_t ids[numThreads];
  int threadIds[numThreads];
  for(int j = 0; j<numThreads; j++) {
    threadIds[j] = j;
  }
  for(int j = 0; j<numThreads; j++) {
    // make new threads from [0,numThreads)
    // put the loop in a function and then j will be used to calculate the start
    // and end of the loop for the thread
    // t0: 2
    pthread_create(&ids[j], NULL, factor, &threadIds[j]);
  }

  for(int j = 0; j<numThreads; j++) {
    pthread_join(ids[j], NULL);
  }
  return 0;
}
