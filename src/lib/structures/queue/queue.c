#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./queue.h"
#include "../../logger.h"

bool isQueueFull(Queue *);

Queue *InitQueue(int capacity)
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));

    if (queue == NULL)
    {
        printf("[ERROR][5XX]: couldn't allocate memory for queue");
        return NULL;
    }

    queue->size = 0;
    queue->max_size = capacity;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

bool isQueueFull(Queue *queue)
{
    return (queue->size == queue->max_size);
}

bool isQueueEmpty(Queue *queue)
{
    return (queue->size == 0);
}

int EnQueue(Queue *queue, int client_socket)
{
    if (isQueueFull(queue))
    {
        (void)Log(WARN, "queue is full\n");
        return QUEUE_SIZE_ERROR;
    }

    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));

    if (node == NULL)
    {
        (void)Log(WARN, "[5XX]: could not allocate space for a new node\n");
        return QUEUE_SIZE_ERROR;
    }

    node->client_socket = client_socket;
    node->next = NULL;

    if (queue->head == NULL)
    {
        queue->head = node;
        queue->tail = node;
        queue->size = 1;

        return queue->size;
    }

    queue->tail->next = node;
    queue->tail = node;
    queue->size += 1;

    return queue->size;
}

int DeQueue(Queue *queue)
{
    if (isQueueEmpty(queue))
    {
        return QUEUE_SIZE_ERROR;
    }

    QueueNode *tmp = NULL;

    int client_socket = queue->head->client_socket;
    tmp = queue->head;
    queue->head = queue->head->next;
    queue->size -= 1;

    free(tmp);

    return client_socket;
}

void FreeQueue(Queue *queue)
{
    if (queue == NULL)
    {
        return;
    }

    while (queue->head != NULL)
    {
        QueueNode *tmp = queue->head;
        queue->head = queue->head->next;
        free(tmp);
    }

    if (queue->tail != NULL)
    {
        free(queue->tail);
    }
    free(queue);
}
