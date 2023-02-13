/*

A program where the 2 producer produce some output and 2 consumers wait for it.

*/
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

int buffer[100];

int loopCount = 5;
int length = 0;

void* producer1(void* arg) {
    int i;
    for (i = 0; i < loopCount; i++) {
        pthread_mutex_lock(&mutex);
        buffer[length++] = i;
        printf("P1 - Added item %d, Producer length %d\n", i, length);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
}

void* producer2(void* arg) {
    int i;
    for (i = 0; i < loopCount; i++) {
        pthread_mutex_lock(&mutex);
        buffer[length++] = i+5;
        printf("P2 - Added item %d, Producer length %d\n", i+5, length);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
}

void* consumer(void* arg) {
    int i;
    for (i = 0; i < 2*loopCount; i++) {
        pthread_mutex_lock(&mutex);
        while (length == 0) {
            printf("Consumer waiting...\n");
            pthread_cond_wait(&cond, &mutex);
        }
        int item = buffer[--length];
        printf("Consumer consumed item %d\n", item);
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char* argv[]) {

    pthread_mutex_init(&mutex, 0);
    pthread_cond_init(&cond, 0);

    pthread_t pThread1, pThread2, cThread;
    pthread_create(&pThread1, 0, producer1, 0);
    pthread_create(&pThread2, 0, producer2, 0);
    pthread_create(&cThread, 0, consumer, 0);
    pthread_join(pThread1, NULL);
    pthread_join(pThread2, NULL);
    pthread_join(cThread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}