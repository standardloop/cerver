#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "./queue/queue.h"

typedef struct
{
    enum ThreadPolicy policy;
    int buffer_size;
    int curr_size;
    Queue *buffer;

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
void FreeThreadPool(ThreadPool *thread_pool);
void StartThreads(Scheduler *, ThreadPool *);
Scheduler *InitScheduler(enum ThreadPolicy, int);
void ScheduleRequestToBeHandled(Scheduler *, ThreadPool *, int);
int AcceptRequest(Scheduler *, ThreadPool *);
void FreeScheduler(Scheduler *);

#endif
