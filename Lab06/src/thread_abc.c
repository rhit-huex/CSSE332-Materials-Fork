/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * @author <Your name>
 * @date   <Date last modified>
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

//         INSTRUCTION
// Use condition variables to make  A, B, C print out in order.
// HINT: You need more than one condition variables

// 1. Define the state of the world
int thread_a_done = 0;
int thread_b_done = 0;

// 2. Conditions variables for signaling on each wait condition
pthread_cond_t a_done_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t b_done_cond = PTHREAD_COND_INITIALIZER;

// 3. Define our mutex lock
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void *
thread_func_A(void *arg)
{
  sleep(3);

  printf("A\n");

  pthread_mutex_lock(&lock);
  thread_a_done = 1;
  pthread_cond_signal(&a_done_cond);
  pthread_mutex_unlock(&lock);

  return NULL;
}

void *
thread_func_B(void *arg)
{
  sleep(2);
  /** NOTES TO SELF ABOUT pthread_cond_wait
   *  When this is called it blocks/sleeps the thread we are in, so the while loop is not constantly running.
   *  So could we use an if statement? Yes, but it is unsafe. Since some architectures allow for threads to
   *  wake up without a signal. So in this case, an if statement would not recheck the global state, but instead
   *  proceed when we do not want it to. A good example of this is in the case of a pthread_broadcast.
   *
   *  When we call wait it releases the lock we specified. And it listens for the signal to the condvar
   *  Once we get the signal, our thread doesn't start immediately, but it waits to reaquire the mutex lock
   *  So after our signal we should release the lock to let our thread run.
   *  What happens if we released the lock before the signal? I think other threads in the scheduler may be able
   *  to use the lock which would potentially cause things to run in an order we do not desire. It may introduce
   *  race conditions for multiple threads competing for the lock.
   *
   *  After wait is signaled the lock is reaquired immediately, which locks the mutex.
   *  We unlock the lock and lock it after the print since the print is slow and we don't want to waste that
   *  time for other threads. Unlocking the lock will allow other threads to use the lock during the print.
   *  This obeys the principle of "hold the lock for as little time possible".
   *  It also allows other threads to access the global state. For example, we just updated the global var for a
   *  but what if we had multiple threads wanting to do something for that. We don't want thread b to block those
   *  threads from doing their operations.
   */
  pthread_mutex_lock(&lock);
  while(!thread_a_done) {
    pthread_cond_wait(&a_done_cond, &lock);
  }
  pthread_mutex_unlock(&lock);

  printf("B\n");

  pthread_mutex_lock(&lock);
  thread_b_done = 1;
  pthread_cond_signal(&b_done_cond);
  pthread_mutex_unlock(&lock);

  return NULL;
}
void *
thread_func_C(void *arg)
{
  pthread_mutex_lock(&lock);
  while(!thread_b_done) {
    pthread_cond_wait(&b_done_cond, &lock);
  }
  pthread_mutex_unlock(&lock);

  printf("C\n");

  return NULL;
}

int
main(int argc, char *argv[])
{
  pthread_t tA, tB, tC;

  pthread_create(&tC, NULL, thread_func_C, NULL);
  pthread_create(&tB, NULL, thread_func_B, NULL);
  pthread_create(&tA, NULL, thread_func_A, NULL);

  // join waits for the threads to finish
  pthread_join(tA, NULL);
  pthread_join(tB, NULL);
  pthread_join(tC, NULL);
  return 0;
}
