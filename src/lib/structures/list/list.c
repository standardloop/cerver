#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "./list.h"

bool isListFull(List *);
bool isListEmpty(List *);

ListNode *newListNode(char *);
void freeListNodeFields(ListNode *);
void freeListNode(ListNode *ListNode);
void freeAllListNodes(ListNode *);

List *InitList(int max)
{
    if (max <= 0)
    {
        return NULL;
    }
    List *list = (List *)malloc(sizeof(List));
    if (list == NULL)
    {
        return NULL;
    }
    list->head = NULL;
    list->count = 0;
    list->max = max;

    return list;
}

ListNode *newListNode(char *value)
{
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    if (node == NULL)
    {
        return NULL;
    }
    node->value = value;
    node->next = NULL;

    return node;
}

bool isListFull(List *list)
{
    return list->count == list->max;
}

bool isListEmpty(List *list)
{
    return ((list->count == 0) && (list->head == NULL));
}

int ListAdd(List *list, char *value)
{
    if (list == NULL)
    {
        return LIST_ERROR_NULL;
    }
    if (isListFull(list))
    {
        return LIST_ERROR_FULL;
    }

    if (isListEmpty(list))
    {
        list->head = newListNode(value);
        if (list->head == NULL)
        {
            return LIST_ERROR_ALLOCATE;
        }
        list->count = 1;
        return 1;
    }

    ListNode *start = list->head;
    ListNode *iterator = start;
    while (iterator != NULL)
    {
        if (iterator->next == NULL && iterator)
        {
            iterator->next = newListNode(value);

            if (iterator->next == NULL)
            {
                return LIST_ERROR_ALLOCATE;
            }
            list->head = start;
            list->count++;
            return list->count;
        }
        iterator = iterator->next;
    }
    return LIST_ERROR_TEMP;
}

void freeListNodeFields(ListNode *node)
{
    if (node == NULL)
    {
        return;
    }
    if (node->value != NULL)
    {
        (void)free(node->value);
    }
}

void freeListNode(ListNode *node)
{
    (void)freeListNodeFields(node);
    (void)free(node);
}

void freeAllListNodes(ListNode *head)
{
    ListNode *temp = NULL;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        (void)freeListNode(temp);
    }
}

void FreeList(List *list)
{
    if (list == NULL)
    {
        return;
    }
    if (list->head != NULL)
    {
        (void)freeAllListNodes(list->head);
    }
    (void)free(list);
}

// FIXME Log support?
void PrintList(List *list)
{
    if (list == NULL)
    {
        return;
    }

    ListNode *iterator = list->head;
    while (iterator != NULL)
    {
        printf("\n%s\n", iterator->value);
        fflush(stdout);
        iterator = iterator->next;
    }
}
