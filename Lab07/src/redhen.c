/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * @author <Your name>
 * @date   <Date last modified>
 */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_LOAVES_PER_BATCH 7
#define NUM_BATCHES 6
#define NUM_LOAVES_TO_EAT 14

/**
 * This system has four threads: the duck, cat, and dog that eat
 * bread, and the little red hen that makes the bread. The little
 * red hen makes seven loaves per batch, but she only has the patience
 * for six batches. The little red hen only makes a batch if there are
 * no loaves left.
 *
 * The other three animals each want to eat 14 loaves of
 * bread, but only one of them can be in the kitchen at a time (to avoid
 * fights over who gets what bread).
 *
 * When the duck, cat, or dog notices that there are no loaves of bread
 * available, they complain to the little red hen and wait (in the kitchen)
 * for the next batch to be ready.
 *
 * Use mutex locks and condition variables to enforce this constraint. Note:
 *the global numLoaves variable should be left as is.
 *
 * look at littleRedHenSampleOutput.txt for an example correct output
 * sequence
 */

// State of the world
int numLoaves = 0;
// int curNumBatches = 0; We dont need to track this
int numInKitchen = 0;

// Condition Variables
pthread_cond_t bread_ready = PTHREAD_COND_INITIALIZER;
pthread_cond_t bread_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t exited_kitchen = PTHREAD_COND_INITIALIZER;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *
littleRedHenThread(void *arg)
{
  char *name = (char *)arg;
  int batch;

  for(batch = 1; batch <= 6; batch++) {
    sleep(2); // just makes it obvious that it won't work without
              // condition variables
    pthread_mutex_lock(&lock);
    while(numLoaves > 0) { // while the bread is empty
      pthread_cond_wait(&bread_empty, &lock);
    } // once the loaves run out then make more
    numLoaves += 7;
    pthread_cond_signal(&bread_ready);
    pthread_mutex_unlock(&lock);
    printf("%-20s: A fresh batch of bread is ready.\n", name);
  }

  printf("%-20s: I'm fed up with feeding you lazy animals! "
         "No more bread!\n",
         name);
  return NULL;
}

void *
otherAnimalThread(void *arg)
{
  char *name         = (char *)arg;
  int numLoavesEaten = 0;
  while(numLoavesEaten < NUM_LOAVES_TO_EAT) {
    // enter the kitchen before waiting for bread
    pthread_mutex_lock(&lock);
    while(numInKitchen != 0) {
      pthread_cond_wait(&exited_kitchen, &lock);
    }

    //pthread_mutex_lock(&lock);
    numInKitchen = 1;

    if(numLoaves <= 0) {
      printf("%-20s: Hey, Little Red Hen, make some more bread!\n", name);
      // Signal the hen to make bread, wait on the bread
      pthread_cond_signal(&bread_empty);
    }

    // We still have the lock bc the last wait gave it back to us
    while(numLoaves <= 0) {
      pthread_cond_wait(&bread_ready, &lock);
    } // once we get some loaves continue
    numLoaves--;

    pthread_mutex_unlock(&lock);
    printf("%-20s: Mmm, this loaf is delicious.\n", name);

    numLoavesEaten++; // local variable we dont need to lock it

    if(random() > random()) { // Adds variety to output
      sleep(1);
    }

    pthread_mutex_lock(&lock);
    // leave the kitchen, I guess we gotta go back and forth for bread
    numInKitchen = 0;
    pthread_cond_signal(&exited_kitchen);
    pthread_mutex_unlock(&lock);
  }
  printf("%-20s: I've had my fill of bread. Thanks, Little Red Hen!\n", name);
  return NULL;
}

int
main(int argc, char **argv)
{
  pthread_t dog, cat, duck, hen;
  numLoaves       = 0;
  char dogName[]  = "Lazy Dog";
  char catName[]  = "Sleepy Cat";
  char duckName[] = "Noisy Yellow Duck";
  char henName[]  = "Little Red Hen";

  pthread_create(&dog, NULL, otherAnimalThread, dogName);
  pthread_create(&cat, NULL, otherAnimalThread, catName);
  pthread_create(&duck, NULL, otherAnimalThread, duckName);
  pthread_create(&hen, NULL, littleRedHenThread, henName);
  pthread_join(dog, NULL);
  pthread_join(cat, NULL);
  pthread_join(duck, NULL);
  pthread_join(hen, NULL);

  printf("Everything finished.\n");
}
