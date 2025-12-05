#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "./../structures/queue/queue.h"
#include "./../http/router.h"

enum ThreadPolicy
{
    FIFO = 0
};

typedef struct
{
    enum ThreadPolicy policy;
    int buffer_size;
    int curr_size;
    Queue *buffer;
    volatile sig_atomic_t cerver_running;

} Scheduler;

typedef struct
{
    int num_threads;
    int working_threads;
    pthread_t *pool;
    pthread_mutex_t LOCK;
    pthread_cond_t FILL;
    pthread_cond_t EMPTY;
    pthread_cond_t SIG;

} ThreadPool;

ThreadPool *InitThreadPool(int);
void ShutdownThreadPool(ThreadPool *thread_pool);
void FreeThreadPool(ThreadPool *thread_pool);
void StartThreads(Router *router, Scheduler *, ThreadPool *);
Scheduler *InitScheduler(enum ThreadPolicy, int);
void ScheduleRequestToBeHandled(Scheduler *, ThreadPool *, int);
void CompleteAlreadyScheduledRequests(Scheduler *, ThreadPool *);
int AcceptRequest(Scheduler *, ThreadPool *);
void FreeScheduler(Scheduler *);

#endif
