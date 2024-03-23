#ifndef WORKER_H
#define WORKER_H

#include "./queue.h"
#include "./scheduler.h"

typedef struct
{

    Scheduler *scheduler;
    ThreadPool *workers;
    int num_request;

} ThreadArg;

void *ThreadWorker(void *arg);
#endif