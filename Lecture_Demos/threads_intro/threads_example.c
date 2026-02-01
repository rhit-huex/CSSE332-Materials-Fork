#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 5

int max;
volatile int counter = 0; // shared global variable

void *
mythread(void *arg)
{
  char *letter = arg;
  int i; // stack (private per thread)
  printf("%s: begin [addr of i: %p]\n", letter, &i);
  printf("%s: begin [addr of count: %p]\n", letter, &counter);
  for(i = 0; i < max; i++) {
    counter = counter + 1; // shared: only one
  }
  printf("%s: done\n", letter);
  return NULL;
}

int
main(int argc, char *argv[])
{
  if(argc != 2) {
    fprintf(stderr, "usage: %s <loopcount>\n", argv[0]);
    exit(1);
  }
  max = atoi(argv[1]);
  pthread_t threads[MAX_THREADS];
  const char* ids[MAX_THREADS] = {"A", "B", "C", "D", "E"};
  for(int i=0; (i<MAX_THREADS); i++) {
    pthread_create(&threads[i], NULL, mythread, ids[i]);
    //pthread_join(threads[i], NULL);
  }
  for(int i=0; (i<MAX_THREADS); i++) {
    pthread_join(threads[i], NULL);
  }


  //pthread_t p1, p2;
  //printf("main: begin [counter = %d]\n", counter);
  //pthread_create(&p1, NULL, mythread, "A");
  //pthread_create(&p2, NULL, mythread, "B");
  // join waits for the threads to finish
  //pthread_join(p1, NULL);
  //pthread_join(p2, NULL);
  printf("main: done\n [counter: %d]\n [should: %d]\n", counter, max * MAX_THREADS);
  return 0;
}
