#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "./map.h"

bool isMapFull(Map *);
bool isMapEmpty(Map *);

MapNode *newMapNode(char *, char *);
void freeMapNodeFields(MapNode *);
void freeMapNode(MapNode *mapNode);
void freeAllMapNodes(MapNode *);

Map *InitMap(int max)
{
    if (max <= 0)
    {
        return NULL;
    }
    Map *map = (Map *)malloc(sizeof(Map));
    if (map == NULL)
    {
        return NULL;
    }
    map->head = NULL;
    map->count = 0;
    map->max = max;

    return map;
}

MapNode *newMapNode(char *key, char *value)
{
    MapNode *mapNode = (MapNode *)malloc(sizeof(MapNode));
    if (mapNode == NULL)
    {
        return NULL;
    }
    mapNode->key = key;
    mapNode->value = value;
    mapNode->next = NULL;

    return mapNode;
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
        map->head = newMapNode(key, value);
        if (map->head == NULL)
        {
            return MAP_ERROR_ALLOCATE;
        }
        map->count = 1;
        return 1;
    }

    MapNode *start = map->head;
    MapNode *iterator = start;
    while (iterator != NULL)
    {
        if (strcmp(key, iterator->key) == 0)
        {
            return MAP_ERROR_PRESENT;
        }
        if (iterator->next == NULL && iterator)
        {
            iterator->next = newMapNode(key, value);

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
    MapNode *iterator = map->head;
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
            MapNode *temp = map->head;
            map->head = map->head->next;
            free(temp);
            map->count--;
            return map->count;
        }
    }

    // guranteed to have more than 1 in list
    MapNode *iterator = map->head;
    MapNode *iterator_next = iterator->next;

    while (iterator_next != NULL)
    {
        if (strcmp(key, iterator_next->key) == 0)
        {
            if (iterator_next->next == NULL)
            {
                freeMapNode(iterator_next);
                map->count--;
                map->head->next = NULL;
                return map->count;
            }
            iterator->next = iterator_next->next;
            freeMapNode(iterator_next);
            map->count--;
            return map->count;
        }
        iterator = iterator_next;
        iterator_next = iterator_next->next;
    }
    return MAP_ERROR_404;
}

void freeMapNodeFields(MapNode *mapNode)
{
    if (mapNode == NULL)
    {
        return;
    }
    if (mapNode->key != NULL)
    {
        free(mapNode->key);
    }
    if (mapNode->value != NULL)
    {
        free(mapNode->value);
    }
}

void freeMapNode(MapNode *mapNode)
{
    freeMapNodeFields(mapNode);
    free(mapNode);
}

void freeAllMapNodes(MapNode *head)
{
    MapNode *temp = NULL;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        freeMapNode(temp);
    }
}

void FreeMap(Map *map)
{
    if (map == NULL)
    {
        return;
    }
    if (map->head != NULL)
    {
        freeAllMapNodes(map->head);
    }
    free(map);
}

// FIXME Log support?
void PrintMap(Map *map)
{
    if (map == NULL)
    {
        return;
    }

    MapNode *iterator = map->head;
    while (iterator != NULL)
    {
        printf("\n%s: %s\n", iterator->key, iterator->value);
        fflush(stdout);
        iterator = iterator->next;
    }
}
