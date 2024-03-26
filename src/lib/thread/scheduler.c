#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#include "./../logger.h"

#include "./scheduler.h"
#include "./queue/queue.h"
#include "./worker.h"

Scheduler *InitScheduler(enum ThreadPolicy policy, int buffer_size)
{
    (void)Log(INFO, "starting InitScheduler!\n");
    Scheduler *scheduler = (Scheduler *)malloc(sizeof(Scheduler));
    if (scheduler == NULL)
    {
        (void)Log(FATAL, "cannot InitScheduler\n");
        return NULL;
    }
    scheduler->policy = policy;
    scheduler->buffer_size = buffer_size;
    scheduler->curr_size = 0;

    if (policy == FIFO)
    {
        scheduler->buffer = InitQueue(buffer_size);
        if (scheduler->buffer == NULL)
        {
            free(scheduler);
            (void)Log(FATAL, "cannot InitQueue\n");
            return NULL;
        }
    }
    return scheduler;
}

void scheduleRequestInQueue(Scheduler *scheduler, int client_socket)
{
    if (scheduler->policy == FIFO)
    {
        int queue_size = EnQueue(scheduler->buffer, client_socket);
        if (queue_size == QUEUE_SIZE_ERROR)
        {
            (void)Log(WARN, "[5XX]: queue size error\n");
        }
        else
        {
            scheduler->curr_size++;
        }
    }
}

int deQueueRequest(Scheduler *scheduler)
{
    int client_socket = -1;
    if (scheduler->policy == FIFO)
    {
        client_socket = DeQueue(scheduler->buffer);
    }
    scheduler->curr_size--;
    return client_socket;
}

bool isSchedulerFull(Scheduler *scheduler)
{
    // Maybe get if larger for some reason?
    return scheduler->curr_size == scheduler->buffer_size;
}

bool isSchedulerEmpty(Scheduler *scheduler)
{
    return scheduler->curr_size == 0;
}

void ScheduleRequestToBeHandled(Scheduler *scheduler, ThreadPool *workers, int client_socket)
{
    int mutex_lock = pthread_mutex_lock(&workers->LOCK);
    if (mutex_lock != 0)
    {
        (void)Log(WARN, "[5XX]: ScheduleRequestToBeHandled mutex_lock\n");
    }
    while (isSchedulerFull(scheduler))
    {
        int cond_wait = pthread_cond_wait(&workers->FILL, &workers->LOCK);
        if (cond_wait != 0)
        {
            (void)Log(WARN, "[5XX]: ScheduleRequestToBeHandled cond_wait\n");
        }
    }
    scheduleRequestInQueue(scheduler, client_socket);
    int empty_signal = pthread_cond_signal(&workers->EMPTY);
    if (empty_signal != 0)
    {
        (void)Log(WARN, "[5XX]: ScheduleRequestToBeHandled empty_signal\n");
    }
    int lock_signal = pthread_mutex_unlock(&workers->LOCK);
    if (lock_signal != 0)
    {
        (void)Log(WARN, "[5XX]: ScheduleRequestToBeHandled lock_signal\n");
    }
}

int AcceptRequest(Scheduler *scheduler, ThreadPool *workers)
{
    int mutex_lock = pthread_mutex_lock(&workers->LOCK);
    if (mutex_lock != 0)
    {
        (void)Log(WARN, "AcceptRequest mutex_lockn");
    }
    while (isSchedulerEmpty(scheduler))
    {
        int thread_wait = pthread_cond_wait(&workers->EMPTY, &workers->LOCK);
        if (thread_wait != 0)
        {
            (void)Log(WARN, "AcceptRequest thread_wait\n");
        }
    }
    int client_socket = deQueueRequest(scheduler);

    // printf("Request Scheduled for FD: %d\n", client_socket);

    int signal_fill = pthread_cond_signal(&workers->FILL);
    if (signal_fill != 0)
    {
        (void)Log(WARN, "AcceptRequest signal_fill\n");
    }
    int signal_lock = pthread_mutex_unlock(&workers->LOCK);
    if (signal_lock != 0)
    {
        (void)Log(WARN, "AcceptRequest signal_lock\n");
    }
    return client_socket;
}

void FreeScheduler(Scheduler *scheduler)
{
    if (scheduler->buffer != NULL)
    {
        FreeQueue(scheduler->buffer);
    }
    free(scheduler);
}
