#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE_ERROR -1

typedef struct node
{
    int client_socket;
    struct node *next;
} Node;

typedef struct queue
{
    int size;
    int max_size;
    Node *head;
    Node *tail;
} Queue;

Queue *InitQueue(int);
int EnQueue(Queue *, int);
int DeQueue(Queue *);
void FreeQueue(Queue *queue);

#endif
