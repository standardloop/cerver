#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdlib.h>
#include <stdio.h>

#include "./queue.h"

typedef struct
{
    enum ThreadPolicy policy;
    size_t buffer_size;
    size_t curr_size;
    Queue *Queue;

} Scheduler;

#endif
