#include <stdlib.h>
#include <stdio.h>
#include "request.h"

// HEAD / HTTP/1.1
// Host: localhost:8080
// User-Agent: curl/7.79.1
// Accept: */*

int getSize(char *buffer, size_t buffer_length)
{
    // FIXME: ADD ERROR LOGIC
    printf("[TRACE]: getSize Char: %c\n", *buffer);
    char *t;
    int size = 0;

    for (*t = buffer; size < MAX_METHOD_LENGTH && size < buffer_length && *t != '/' && *t != '\0'; t++)
    {
        printf("[TRACE]: getSize Char: %c\n", *t);
        size++;
    }
    return size;
}

enum HttpMethods ParseRequestMethod(char *buffer, size_t buffer_length)
{
    printf("[TRACE]: entering a new function\n");
    int size = getSize(buffer, buffer_length);
    printf("[DEBUG]: size till:  %d\n", size);
    if (size == 0)
    {
        return HttpFAKER;
    }

    return HttpFAKER;
}

HttpRequest *CreateHttpRequest()
{
    return NULL;
}
