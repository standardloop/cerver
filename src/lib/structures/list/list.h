#ifndef LIST_H
#define LIST_H

#define LIST_ERROR_PRESENT -1
#define LIST_ERROR_FULL -2
#define LIST_ERROR_ALLOCATE -3
#define LIST_ERROR_TEMP -4
#define LIST_ERROR_NULL -5
#define LIST_ERROR_EMPTY -6
#define LIST_ERROR_404 -7

typedef struct listNode
{
    struct listNode *next;
    char *value;
} ListNode;

typedef struct
{
    ListNode *head;
    int count;
    int max;
} List;

List *InitList(int);
int ListAdd(List *, char *);

void PrintList(List *);
void FreeList(List *List);

#endif
