#ifndef POOL_H
#define POOL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct
{
    int num_threads;
    int working_threads;
    pthread_t *pool;
    pthread_mutex_t LOCK;
    pthread_cond_t FILL;
    pthread_cond_t EMPTY;

} ThreadPool;

ThreadPool *InitThreadPool(int);
#endif
