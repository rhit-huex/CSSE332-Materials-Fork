
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


//         INSTRUCTION
// Use condition variables to make  A, B, C print out in order.
// HINT: You need more than one condition variables

// State of the world
int thread_a_done = 0;
int thread_b_done = 0;

// Condition variables - one convar per wait condition
pthread_cond_t a_done_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t b_done_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* thread_func_A(void* arg) {
    sleep(3);

    printf("A\n");

    pthread_mutex_lock(&lock);
    thread_a_done = 1;
    pthread_cond_signal(&a_done_cond);
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* thread_func_B(void* arg) {
    sleep(2);

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
void* thread_func_C(void* arg) {
    pthread_mutex_lock(&lock);

    while(!thread_b_done) {
      pthread_cond_wait(&b_done_cond, &lock);
    }
    pthread_mutex_unlock(&lock);
    printf("C\n");

    return NULL;
}

int main(int argc, char *argv[]) {
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
