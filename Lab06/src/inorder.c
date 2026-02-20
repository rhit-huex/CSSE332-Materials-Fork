/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * @author <Your name>
 * @date   <Date last modified>
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

// State of the world
int next_to_exit = 1; // 1 gets to go first

// Conditional Variables
pthread_cond_t thread_exited = PTHREAD_COND_INITIALIZER;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *
thread(void *arg)
{
  int *num = (int *)arg;
  printf("%d wants to enter the critical section\n", *num);

  // simulate the critical section with a simple sleep.
  sleep(1);

  // Grab lock
  pthread_mutex_lock(&lock);
  while(*num != next_to_exit) {
    pthread_cond_wait(&thread_exited, &lock);
  }
  pthread_mutex_unlock(&lock);

  pthread_mutex_lock(&lock);
  next_to_exit += 1;
  pthread_cond_broadcast(&thread_exited);
  pthread_mutex_unlock(&lock);
  printf("%d is finished with the critical section\n", *num);

  return NULL;
}

int
main(int argc, char **argv)
{
  pthread_t threads[4];
  int i;
  int nums[] = {2, 1, 4, 3};

  for(i = 0; i < 4; ++i) {
    pthread_create(&threads[i], NULL, thread, &nums[i]);

    if(i == 2)
      sleep(3);
  }

  for(i = 0; i < 4; ++i) {
    pthread_join(threads[i], NULL);
  }

  printf("Everything finished\n");

  return 0;
}
