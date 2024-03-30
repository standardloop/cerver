#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "./map.h"

bool isMapFull(Map *);
bool isMapEmpty(Map *);

Node *newNode(char *, char *);
void freeNodeFields(Node *);
void freeNode(Node *node);
void freeAllNodes(Node *);

Map *InitMap(int max)
{
    if (max <= 0)
    {
        return NULL;
    }
    Map *map = malloc(sizeof(Map));
    if (map == NULL)
    {
        return NULL;
    }
    map->head = NULL;
    map->count = 0;
    map->max = max;

    return map;
}

Node *newNode(char *key, char *value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
    {
        return NULL;
    }
    node->key = key;
    node->value = value;
    node->next = NULL;

    return node;
}

bool isMapFull(Map *map)
{
    return map->count == map->max;
}

bool isMapEmpty(Map *map)
{
    return ((map->count == 0) && (map->head == NULL));
}

int MapAdd(Map *map, char *key, char *value)
{
    if (map == NULL)
    {
        return MAP_ERROR_NULL;
    }
    if (isMapFull(map))
    {
        return MAP_ERROR_FULL;
    }

    if (isMapEmpty(map))
    {
        map->head = newNode(key, value);
        if (map->head == NULL)
        {
            return MAP_ERROR_ALLOCATE;
        }
        map->count = 1;
        return 1;
    }

    Node *start = map->head;
    Node *iterator = start;
    while (iterator != NULL)
    {
        if (strcmp(key, iterator->key) == 0)
        {
            return MAP_ERROR_PRESENT;
        }
        if (iterator->next == NULL && iterator)
        {
            iterator->next = newNode(key, value);

            if (iterator->next == NULL)
            {
                return MAP_ERROR_ALLOCATE;
            }
            map->head = start;
            map->count++;
            return map->count;
        }
        iterator = iterator->next;
    }
    return MAP_ERROR_TEMP;
}

char *MapGet(Map *map, char *key)
{
    if (map == NULL || isMapEmpty(map))
    {
        return NULL;
    }
    Node *iterator = map->head;
    while (iterator != NULL)
    {
        if (strcmp(key, iterator->key) == 0)
        {
            return iterator->value;
        }
        iterator = iterator->next;
    }
    return NULL;
}

int MapRemove(Map *map, char *key)
{
    if (map == NULL || key == NULL)
    {
        return MAP_ERROR_NULL;
    }
    if (isMapEmpty(map) || map->head == NULL)
    {
        return MAP_ERROR_EMPTY;
    }
    if (strcmp(key, map->head->key) == 0)
    {
        if (map->count == 1)
        {
            free(map->head);
            map->head = NULL;
            map->count = 0;
            return 0;
        }

        if (map->head->next != NULL) // FIXME ALWAYS?
        {
            Node *temp = map->head;
            map->head = map->head->next;
            free(temp);
            map->count--;
            return map->count;
        }
    }

    // guranteed to have more than 1 in list
    Node *iterator = map->head;
    Node *iterator_next = iterator->next;

    while (iterator_next != NULL)
    {
        if (strcmp(key, iterator_next->key) == 0)
        {
            if (iterator_next->next == NULL)
            {
                freeNode(iterator_next);
                map->count--;
                map->head->next = NULL;
                return map->count;
            }
            iterator->next = iterator_next->next;
            freeNode(iterator_next);
            map->count--;
            return map->count;
        }
        iterator = iterator_next;
        iterator_next = iterator_next->next;
    }
    return MAP_ERROR_404;
}

void freeNodeFields(Node *node)
{
    if (node == NULL)
    {
        return;
    }
    if (node->key != NULL)
    {
        free(node->key);
    }
    if (node->value != NULL)
    {
        free(node->value);
    }
}

void freeNode(Node *node)
{
    freeNodeFields(node);
    free(node);
}

void freeAllNodes(Node *head)
{
    Node *temp = NULL;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        freeNode(temp);
    }
}

void FreeMap(Map *map)
{
    if (map == NULL)
    {
        return;
    }
    freeAllNodes(map->head);
    free(map);
}

// FIXME Log support?
void PrintMap(Map *map)
{
    if (map == NULL)
    {
        return;
    }

    Node *iterator = map->head;
    while (iterator != NULL)
    {
        printf("\n%s : %s\n", iterator->key, iterator->value);
        fflush(stdout);
        iterator = iterator->next;
    }
}
