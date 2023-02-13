#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

int i;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void* thread_func() {
    //  int i = 0;
    while (1) {
        pthread_mutex_lock(&mtx);
        printf("child thread: %d\n", i++);
        pthread_mutex_unlock(&mtx);
        sleep(1);
    }
}
int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread_func, NULL);
    pthread_create(&t2, NULL, thread_func, NULL);
    //int i = 0;
    while (1) {
        pthread_mutex_lock(&mtx);
        printf("main thread: %d\n", i++);
        pthread_mutex_unlock(&mtx);
        sleep(1);
    }
}