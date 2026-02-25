/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * @author <Your name>
 * @date   <Date last modified>
 */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

// number of carpenters
#define NUM_CARP 3
// number of painters
#define NUM_PAIN 3
// number of decorators
#define NUM_DECO 3

/**
 * Imagine there is a shared memory space called house.
 *
 * There are 3 different kinds of operations on house: carpenters, painters, and
 * decorators.  For any particular kind of operation, there can be an unlimited
 * number of threads doing the same operation at once (e.g. unlimited carpenter
 * threads etc.).  However, only one kind of operation can be done at a time (so
 * even a single carpenter should block all painters and vice versa).
 *
 * Use mutex locks and condition variables to enforce this constraint.  You
 * don't have to worry about starvation (e.g. that constantly arriving
 * decorators might prevent carpenters from ever running) - though maybe it
 * would be fun to consider how you would solve in that case.
 *
 */

// State of the world
int current_func   = -1; // 1:free 0:carpentry 1:painting 2:decorating
int num_carpenters = 0;
int num_painters   = 0;
int num_decorators = 0;

// Condition variables
// We need to broadcast signals to all threads since there is a race to get the lock
// (the lock prevents the race condition, which would run a thread with the wrong current_func val)
// So we can have 3 condvars for each process that broadcast
// Then which ever gets it first will change the current_func and then all threads will re-check
// their while loops.
// Actually, I don't want to signal everytime for each thread, so I need to track counts and then
// We should only need 1 cond var because we intend to broadcast our signal
pthread_cond_t job_finish_cond  = PTHREAD_COND_INITIALIZER;
//pthread_cond_t painting_cond   = PTHREAD_COND_INITIALIZER;
//pthread_cont_t decorating_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *
carpenter(void *ignored)
{
  pthread_mutex_lock(&lock);

  num_carpenters += 1; // everytime we make this thread, start by adding it to count
  while(current_func != -1 && current_func != 0) { // wait while we are not free and not carpentry
    pthread_cond_wait(&job_finish_cond, &lock);
  }
  current_func = 0; // we are doing carpentry now
  pthread_mutex_unlock(&lock);

  printf("starting carpentry\n");
  sleep(1);
  printf("finished carpentry\n");

  pthread_mutex_lock(&lock);
  num_carpenters -= 1;
  if(num_carpenters <= 0) {
    current_func = -1;
    pthread_cond_broadcast(&job_finish_cond);
  }
  pthread_mutex_unlock(&lock);
  return NULL;
}

void *
painter(void *ignored)
{
  pthread_mutex_lock(&lock);
  num_painters += 1;
  while(current_func != -1 && current_func != 1) {
    pthread_cond_wait(&job_finish_cond, &lock);
  }
  current_func = 1; // We are painting now
  pthread_mutex_unlock(&lock);

  printf("starting painting\n");
  sleep(1);
  printf("finished painting\n");

  pthread_mutex_lock(&lock);
  num_painters -= 1;
  if(num_painters <= 0) {
    current_func = -1;
    pthread_cond_broadcast(&job_finish_cond);
  }
  pthread_mutex_unlock(&lock);
  return NULL;
}

void *
decorator(void *ignored)
{
  pthread_mutex_lock(&lock);
  num_decorators += 1;
  while(current_func != -1 && current_func != 2) {
    pthread_cond_wait(&job_finish_cond, &lock);
  }
  current_func = 2; // We are decorating now
  pthread_mutex_unlock(&lock);
  printf("starting decorating\n");
  sleep(1);
  printf("finished decorating\n");

  pthread_mutex_lock(&lock);
  num_decorators -= 1;
  if(num_decorators <= 0) {
    current_func = -1;
    pthread_cond_broadcast(&job_finish_cond);
  }
  pthread_mutex_unlock(&lock);
  return NULL;
}

int
main(int argc, char **argv)
{
  pthread_t jobs[NUM_CARP + NUM_PAIN + NUM_DECO];
  for(int i = 0; i < NUM_CARP + NUM_PAIN + NUM_DECO; i++) {
    void *(*func)(void *) = NULL;
    if(i < NUM_CARP)
      func = carpenter;
    if(i >= NUM_CARP && i < NUM_CARP + NUM_PAIN)
      func = painter;
    if(i >= NUM_CARP + NUM_PAIN) {
      func = decorator;
    }
    pthread_create(&jobs[i], NULL, func, NULL);
  }

  for(int i = 0; i < NUM_CARP + NUM_PAIN + NUM_DECO; i++) {
    pthread_join(jobs[i], NULL);
  }

  printf("Everything finished.\n");
}
