#include <stdio.h>
#include <stdlib.h>

#include "./scheduler.h"
#include "./queue.h"
#include "./worker.h"

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
int EnQueue(Queue *queue, int *client_socket)
{
    if ((queue->size + 1) > queue->max_size)
    {
        printf("\n[WARN][5XX]: queue is full\n");
        return queue->size;
    }

    Node *node = malloc(sizeof(Node));

    if (node == NULL)
    {
        printf("\n[WARN][5XX]: could not allocate space for a new node\n");
        return queue->size;
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

int *DeQueue(Queue *queue)
{
    if (queue->size == 0)
    {
        return NULL;
    }

    int *client_socket = NULL;
    Node *tmp = NULL;

    client_socket = queue->head->client_socket;
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
        Node *tmp = queue->head;
        queue->head = queue->head->next;
        if (tmp->client_socket != NULL)
        {
            free(tmp->client_socket);
        }

        free(tmp);
    }

    if (queue->tail != NULL)
    {
        free(queue->tail);
    }

    free(queue);
}
