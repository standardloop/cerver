#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int *client_socket;
    struct node *next;
} Node;

typedef struct queue
{
    int size;
    int max_size;
    Node *head;
    Node *tail;
} Queue;

enum ThreadPolicy
{
    FIFO = 0
};

Queue *InitQueue(int);
int EnQueue(Queue *, int *);
int *DeQueue(Queue *);
void FreeQueue(Queue *queue);

#endif
