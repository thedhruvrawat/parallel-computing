#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>

#define NTHREAD 10
#define MAX_NUM 1000

void* assign_random(void*);
typedef struct mylib_rwlock_t mylib_rwlock_t;
void mylib_rwlock_init(mylib_rwlock_t*);
void mylib_rwlock_rlock(mylib_rwlock_t* l);
void mylib_rwlock_wlock(mylib_rwlock_t* l);
void mylib_rwlock_unlock(mylib_rwlock_t* l);

mylib_rwlock_t myLock;

int global_min = __INT_MAX__;

int main(int argc, char const* argv[]) {
    mylib_rwlock_init(&myLock);
    pthread_t threadArr[NTHREAD];
    int threadIdArr[NTHREAD];
    for (int j = 0; j < NTHREAD; j++) {
        threadIdArr[j] = j;
    }

    int i = 0;
    for (i = 0; i < NTHREAD; i++) {
        pthread_create(&threadArr[i], NULL, &assign_random, &threadIdArr[i]);
    }

    for (int j = 0; j < NTHREAD; ++j) {
        pthread_join(threadArr[j], NULL);
    }
    printf("The smallest value is %d\n", global_min);
    return 0;
}

void* assign_random(void* param) {
    int threadId = *(int*)param;
    int randNum = 0;
    randNum = rand() % MAX_NUM;
    printf("Thread id %d\tRandom Number: %d\n", threadId, randNum);
    
    mylib_rwlock_wlock(&myLock);
    if(randNum<global_min)
        global_min = randNum;
    mylib_rwlock_unlock(&myLock);
    return NULL;
}

struct mylib_rwlock_t{
    int readers;
    int writer;
    pthread_cond_t readers_proceed;
    pthread_cond_t writer_proceed;
    int pending_writers;
    pthread_mutex_t read_write_lock;
};

void mylib_rwlock_init(mylib_rwlock_t* l) {
    l->readers = l->writer = l->pending_writers = 0;
    pthread_mutex_init(&(l->read_write_lock), NULL);
    pthread_cond_init(&(l->readers_proceed), NULL);
    pthread_cond_init(&(l->writer_proceed), NULL);
}

void mylib_rwlock_rlock(mylib_rwlock_t* l) {
    /* if there is a write lock or pending writers, perform condition
    wait.. else increment count of readers and grant read lock */

    pthread_mutex_lock(&(l->read_write_lock));
    while ((l->pending_writers > 0) || (l->writer > 0)) {
        pthread_cond_wait(&(l->readers_proceed), &(l->read_write_lock));
    }
    l->readers++;
    pthread_mutex_unlock(&(l->read_write_lock));
}


void mylib_rwlock_wlock(mylib_rwlock_t* l) {
    /* if there are readers or writers, increment pending writers
    count and wait. On being woken, decrement pending writers
    count and increment writer count */

    pthread_mutex_lock(&(l->read_write_lock));
    while ((l->writer > 0) || (l->readers > 0)) {
        l->pending_writers++;
        pthread_cond_wait(&(l->writer_proceed),
            &(l->read_write_lock));
    }
    l->pending_writers--;
    l->writer++;
    pthread_mutex_unlock(&(l->read_write_lock));
}


void mylib_rwlock_unlock(mylib_rwlock_t* l) {
    /* if there is a write lock then unlock, else if there are
    read locks, decrement count of read locks. If the count
    is 0 and there is a pending writer, let it through, else
    if there are pending readers, let them all go through */

    pthread_mutex_lock(&(l->read_write_lock));
    if (l->writer > 0)
        l->writer = 0;
    else if (l->readers > 0)
        l->readers--;
    pthread_mutex_unlock(&(l->read_write_lock));
    if ((l->readers == 0) && (l->pending_writers > 0))
        pthread_cond_signal(&(l->writer_proceed));
    else if (l->readers > 0)
        pthread_cond_broadcast(&(l->readers_proceed));
}