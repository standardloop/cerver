#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#include "./scheduler.h"
#include "./queue.h"
#include "./worker.h"

Scheduler *InitScheduler(enum ThreadPolicy policy, int buffer_size)
{
    printf("\n[INFO]: starting InitScheduler!\n");
    Scheduler *scheduler = (Scheduler *)malloc(sizeof(Scheduler));
    if (scheduler == NULL)
    {
        printf("\n[FATAL]: cannot InitScheduler\n");
        return NULL;
    }
    scheduler->policy = policy;
    scheduler->buffer_size = buffer_size;
    scheduler->curr_size = 0;

    if (policy == FIFO)
    {
        scheduler->queue = InitQueue(buffer_size);
        if (scheduler->queue == NULL)
        {
            printf("\n[FATAL]: cannot InitQueue\n");
            return NULL;
        }
    }
    return scheduler;
}

void ScheduleNewRequest(Scheduler *scheduler, int *client_socket)
{
    if (scheduler->policy == FIFO)
    {
        int queue_size = EnQueue(scheduler->queue, client_socket);
        scheduler->curr_size++;
        printf("\n[INFO]:ScheduleNewRequest QueueSize: %d\n", queue_size);
    }
}

int *PickRequest(Scheduler *scheduler)
{
    int *client_socket = NULL;
    if (scheduler->policy == FIFO)
    {
        client_socket = DeQueue(scheduler->queue);
    }
    scheduler->curr_size--;
    printf("\n[INFO]:ScheduleNewRequest QueueSize: %d\n", scheduler->curr_size);
    return client_socket;
}

bool IsSchedulerFull(Scheduler *scheduler)
{
    return scheduler->curr_size == scheduler->buffer_size;
}

bool IsSchedulerEmpty(Scheduler *scheduler)
{
    return scheduler->curr_size == 0;
}

void AddToScheduler(Scheduler *scheduler, ThreadPool *workers, int *client_socket)
{

    int mutex_lock = pthread_mutex_lock(&workers->LOCK);
    if (mutex_lock != 0)
    {
        printf("\n[WARN][5XX]: AddToScheduler mutex_lock \n");
    }
    while (IsSchedulerFull(scheduler))
    {
        int cond_wait = pthread_cond_wait(&workers->FILL, &workers->LOCK);
        if (cond_wait != 0)
        {
            printf("\n[WARN][5XX]: AddToScheduler cond_wait \n");
        }
    }
    ScheduleNewRequest(scheduler, client_socket);
    int empty_signal = pthread_cond_signal(&workers->EMPTY);
    if (empty_signal != 0)
    {
        printf("\n[WARN][5XX]: AddToScheduler empty_signal \n");
    }
    int lock_signal = pthread_mutex_unlock(&workers->LOCK);
    if (lock_signal != 0)
    {
        printf("\n[WARN][5XX]: AddToScheduler lock_signal \n");
    }
}

int *GetFromScheduler(Scheduler *scheduler, ThreadPool *workers)
{

    int mutex_lock = pthread_mutex_lock(&workers->LOCK);
    if (mutex_lock != 0)
    {
        printf("\n GetFromScheduler mutex_lock \n");
    }
    while (IsSchedulerEmpty(scheduler))
    {
        int thread_wait = pthread_cond_wait(&workers->EMPTY, &workers->LOCK);
        if (thread_wait != 0)
        {
            printf("\n GetFromScheduler thread_wait \n");
        }
    }
    int *client_socket = PickRequest(scheduler);

    printf("Request Scheduled for FD: %d\n", *client_socket);

    int signal_fill = pthread_cond_signal(&workers->FILL);
    if (signal_fill != 0)
    {
        printf("\n GetFromScheduler signal_fill \n");
    }
    int signal_lock = pthread_mutex_unlock(&workers->LOCK);
    if (signal_lock != 0)
    {
        printf("\n GetFromScheduler signal_lock \n");
    }
    return client_socket;
}

void FreeScheduler(Scheduler *scheduler)
{
    if (scheduler->queue != NULL)
    {
        FreeQueue(scheduler->queue);
    }
    free(scheduler);
}
