#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "./queue.h"

typedef struct
{
    enum ThreadPolicy policy;
    size_t buffer_size;
    size_t curr_size;
    Queue *queue;

} Scheduler;

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
void StartThreads(Scheduler *, ThreadPool *);

Scheduler *InitScheduler(enum ThreadPolicy, int);
void AddToScheduler(Scheduler *, ThreadPool *, int *);
int *GetFromScheduler(Scheduler *, ThreadPool *);
#endif
