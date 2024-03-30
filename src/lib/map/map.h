#ifndef MAP_H
#define MAP_H

#define MAP_ERROR_PRESENT -1
#define MAP_ERROR_FULL -2
#define MAP_ERROR_ALLOCATE -3
#define MAP_ERROR_TEMP -4
#define MAP_ERROR_NULL -5
#define MAP_ERROR_EMPTY -6
#define MAP_ERROR_404 -7

typedef struct node
{
    struct node *next;
    char *key;
    char *value;
} Node;

typedef struct
{
    Node *head;
    int count;
    int max;
} Map;

Map *InitMap(int);

int MapAdd(Map *, char *, char *);
char *MapGet(Map *, char *);
int MapRemove(Map *, char *);

void PrintMap(Map *);
void FreeMap(Map *map);

#endif
