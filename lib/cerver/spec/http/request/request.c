#include <stdlib.h>
#include <stdio.h>
#include "request.h"
#include "../methods/methods.h"

// HEAD / HTTP/1.1
// Host: localhost:8080
// User-Agent: curl/7.79.1
// Accept: */*

int getSize(char *buffer, size_t buffer_length)
{
    // FIXME: ADD ERROR LOGIC
    char *t;
    int size = 0;

    for (t = buffer; (size < buffer_length) || *t != '\0' || *t != '/'; t++)
    {
        size++;
    }
    return size;
}

int ParseRequestMethod(char *buffer, size_t buffer_length)
{
    int size = getSize(buffer, buffer_length);
    printf("[DEBUG]: size till:  %d\n", size);
    if (size == 0 || size > MAX_METHOD_LENGTH)
    {
        return HttpFAKER;
    }

    return HttpFAKER;
}

HttpRequest *CreateHttpRequest()
{
    return NULL;
}
