#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *
thread_fn(void *arg)
{
  int *ptr = (int *)arg;

  printf("Hello from thread %d\n", *ptr);
  // change the id for some reason
  *ptr = 10;

  // sleep works on a thread level, so safe to call it from here
  sleep(5);

  printf("What is my id now? %d\n", *ptr);
  return 0;
}

int
main(int argc, char **argv)
{
  pthread_t threads[10];

  for(int i = 0; i < 10; i++) {
    printf("Creating thread %d\n", i);
    pthread_create(&threads[i], NULL, thread_fn, &i);
    sleep(1);
  }

  // loop that doesn't do much.
  for(int i = 0; i < 20; i++)
    ;

  for(int i = 0; i < 10; i++) {
    pthread_join(threads[i], NULL);
  }
  return 0;
}
