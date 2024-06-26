#ifndef WORKER_H
#define WORKER_H

#include "./../structures/queue/queue.h"
#include "./scheduler.h"
#include "./../http/router.h"

typedef struct
{
    Scheduler *scheduler;
    ThreadPool *workers;
    int worker_number;
    Router *router;
} ThreadArg;

void *ThreadWorker(void *arg);
#endif
