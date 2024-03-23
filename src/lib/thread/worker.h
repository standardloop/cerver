#ifndef WORKER_H
#define WORKER_H

#include "./queue/queue.h"
#include "./scheduler.h"

typedef struct
{
    Scheduler *scheduler;
    ThreadPool *workers;
    int worker_number;
} ThreadArg;

void *ThreadWorker(void *arg);
#endif
