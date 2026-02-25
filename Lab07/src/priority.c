/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * @author <Your name>
 * @date   <Date last modified>
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

/**
 * In this system there are threads numbered 1-6 and a critical
 * section.  The thread numbers are priorities, where thread 6 is
 * highest priority, thread 5 is next, etc.
 *
 * If the critical section is empty, any thread can enter.  While a
 * thread is in the critical section no other threads can enter -
 * regardless of priority. When a thread leaves the critical section
 * and there are threads waiting, the highest priority waiting thread
 * is allowed to enter the critical section.
 *
 * Be sure a newly arriving thread can't jump into the critical
 * section as the current thread finishes, bypassing priority rules.
 * Solve this problem with mutexes/condition variables
 */

/*
  This seems like the same problem as inorder.c from lab06. So ill just do that
  but now choose to run the higher number threads first
*/
// State of the world
int cur_priority = 6; // priority systems usually use a set range so this can be set on initing
int count[6] = {0, 0, 0, 0, 0, 0};
// Conditional Variables
pthread_cond_t thread_exited = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *
thread(void *arg)
{
  int *num = (int *)arg;
  printf("%d wants to enter the critical section\n", *num);

  pthread_mutex_lock(&lock);
  count[*num] += 1; // number of threads in that priority
  while(*num < cur_priority) {
    pthread_cond_wait(&thread_exited, &lock);
  }
  pthread_mutex_unlock(&lock);
  printf("%d has entered the critical section\n", *num);
  sleep(1);
  pthread_mutex_lock(&lock);

  count[*num] -= 1; // number of threads in that prio decreases

  // get the next priority
  if(count[*num] == 0) { // if we are done w/ all threads in the current prio
    cur_priority = *num - 1;
    while(cur_priority > 0 && count[cur_priority] == 0) { // until we are at prio 0 or the next prio
      cur_priority--;
    }
    pthread_cond_broadcast(&thread_exited);
  }

  pthread_mutex_unlock(&lock);
  printf("%d is finished with the critical section\n", *num);

  return NULL;
}

int
main(int argc, char **argv)
{
  int i;
  pthread_t threads[6];
  int nums[] = {2, 1, 4, 3, 5, 6};

  for(i = 0; i < 6; i++) {
    pthread_create(&threads[i], NULL, thread, &nums[i]);

    if(i == 2)
      sleep(10);
  }

  for(i = 0; i < 6; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Everything finished.\n");
}
