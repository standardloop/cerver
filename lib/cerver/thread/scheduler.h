#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdlib.h>
#include <stdio.h>

#include "./queue.h"
#include "./pool.h"

typedef struct
{
    enum ThreadPolicy policy;
    size_t buffer_size;
    size_t curr_size;
    Queue *queue;

} Scheduler;

Scheduler *InitScheduler(enum ThreadPolicy, int);
void AddToScheduler(ThreadPool *, Scheduler *, int *);
int *GetFromScheduler(ThreadPool *, Scheduler *);
#endif
