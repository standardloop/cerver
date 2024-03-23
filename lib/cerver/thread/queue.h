#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int val;
} Node;

typedef struct
{
    Node *array;
} Queue;

enum ThreadPolicy
{
    FIFO = 0
};



#endif
