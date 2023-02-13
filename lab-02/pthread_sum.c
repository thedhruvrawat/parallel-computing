#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void* sum_thread(void* param);
typedef struct mylib_barrier_t mylib_barrier_t;
void mylib_init_barrier(mylib_barrier_t* b);
void mylib_barrier(mylib_barrier_t* b, int num_threads);

#define NTHREAD 12
#define NELEMENTS 60

int arr[] = {
    73, 36, 5, 89, 64, 53, 40, 60, 90, 99, 59, 84, 12, 18, 32, 83, 100, 16, 4, 30, 20, 23, 38, 74, 33, 8, 27, 24, 78, 86, 47, 72, 54, 50, 39, 31, 28, 44, 66, 70, 92, 57, 56, 93, 19, 85, 55, 11, 61, 9, 69, 68, 96, 41, 58, 2, 94, 46, 42, 45
};

int sum = 0;

struct mylib_barrier_t {
    pthread_mutex_t count_lock;
    pthread_cond_t ok_to_proceed;
    int count;
};

mylib_barrier_t myBarrier;
int t[NTHREAD];

int main(int argc, char const* argv[]) {
    pthread_t threadArr[NTHREAD];
    int threadIdArr[NTHREAD];
    for (int j = 0; j < NTHREAD; j++) {
        threadIdArr[j] = j;
    }

    mylib_init_barrier(&myBarrier);

    int i = 0;
    for (i = 0; i < NTHREAD; i++) {
        pthread_create(&threadArr[i], NULL, &sum_thread, &threadIdArr[i]);
    }

    for (int j = 0; j < NTHREAD; ++j) {
        pthread_join(threadArr[j], NULL);
    }
    printf("The sum calculated is: %d\n", sum);
    return 0;
}

void* sum_thread(void* param) {
    int perThreadEl = NELEMENTS/NTHREAD;
    int threadId = *(int*)param;
    int startEl = perThreadEl*threadId;
    int endEl = perThreadEl*(threadId+1)-1;
    sleep(0.5);
    int localSum = 0;
    for(int i=startEl; i<=endEl; i++) {
        localSum+=arr[i];
    }
    t[threadId] = localSum;
    mylib_barrier(&myBarrier, NTHREAD);
    pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&printMutex);
    sum+=localSum;
    printf("At threadId %d, value of localSum: %d\n", threadId, localSum);
    pthread_mutex_unlock(&printMutex);
    return NULL;
}


void mylib_init_barrier(mylib_barrier_t* b) {
    b->count = 0;
    pthread_mutex_init(&(b->count_lock), NULL);
    pthread_cond_init(&(b->ok_to_proceed), NULL);
}

void mylib_barrier(mylib_barrier_t* b, int num_threads) {
    pthread_mutex_lock(&(b->count_lock));
    b->count++;
    if (b->count == num_threads) {
        b->count = 0;
        pthread_cond_broadcast(&(b->ok_to_proceed));
    }
    else
        while (pthread_cond_wait(&(b->ok_to_proceed), &(b->count_lock)) != 0);
    pthread_mutex_unlock(&(b->count_lock));
}