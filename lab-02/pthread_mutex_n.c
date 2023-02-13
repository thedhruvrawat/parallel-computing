#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define n 500

void* mutex_function();
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  counter = 0;

int main() {
    for(int i=0; i<n; i++) {
        int rc;
        int count = i+1;
        pthread_t thread;
        if((rc = pthread_create(&thread, NULL, &mutex_function, (void*)&count))) {
            printf("Thread creation failed: %d\n", rc);
        }
        pthread_join(thread, NULL);
    }
    exit(0);
}

void* mutex_function(int* param) {
    pthread_mutex_lock(&mutex1);
    counter++;
    printf("I'm in thread %d, tid: %d, Counter value: %d\n", *param, gettid(), counter);
    pthread_mutex_unlock(&mutex1);
}