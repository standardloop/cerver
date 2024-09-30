#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE_ERROR -1

typedef struct queue_node
{
    int client_socket;
    struct queue_node *next;
} QueueNode;

typedef struct queue
{
    int size;
    int max_size;
    QueueNode *head;
    QueueNode *tail;
} Queue;

Queue *InitQueue(int);
int EnQueue(Queue *, int);
int DeQueue(Queue *);
void FreeQueue(Queue *queue);

#endif
