#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "./map.h"

Node *newNode(char *, char *);
bool isMapFull(Map *);
bool isMapEmpty(Map *);
void freeNodeFields(Node *);
void freeAllNodes(Node *);

Map *InitMap(int max)
{
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
    if (isMapEmpty(map))
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

void MapRemove(Map *map, char *key)
{
    if (isMapEmpty(map) || key == NULL)
    {
        return;
    }
    if (strcmp(key, map->head->key) == 0)
    {
        Node *temp = map->head;
        if (map->count != 1)
        {
            map->head = map->head->next;
        }
        freeNodeFields(temp);
        free(temp);
        map->count = 0;
        return;
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
                freeNodeFields(iterator_next);
                free(iterator_next);
                map->count--;
                return;
            }
            iterator->next = iterator_next->next;
            freeNodeFields(iterator_next);
            free(iterator_next);
            map->count--;
            return;
        }
        iterator = iterator_next;
        iterator_next = iterator_next->next;
    }
    return;
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

void freeAllNodes(Node *head)
{
    Node *temp = NULL;
    while (head != NULL)
    {
        freeNodeFields(head);
        temp = head;
        head = head->next;
        free(temp);
    }
}
void FreeMap(Map *map)
{
    freeAllNodes(map->head);
    free(map);
}

// FIXME Log support?
void PrintMap(Map *map)
{
    Node *iterator = map->head;

    while (iterator != NULL)
    {
        printf("\n%s : %s\n", iterator->key, iterator->value);
        iterator = iterator->next;
    }
}
