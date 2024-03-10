#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "request.h"
#include "../methods/methods.h"

// HEAD / HTTP/1.1
// Host: localhost:8080
// User-Agent: curl/7.79.1
// Accept: */*

// size_t getMethodStrSize(char *, size_t);

size_t getMethodStrSize(char *buffer, size_t buffer_length)
{
    // FIXME: ADD ERROR LOGIC
    char *temp_ptr;
    size_t size = 0;

    for (temp_ptr = buffer; size < MAX_METHOD_LENGTH && ((sizeof(char)) * size) < buffer_length && *temp_ptr != '/' && *temp_ptr != '\0'; temp_ptr++)
    {
        // printf("[TRACE]: getSize Char: %c\n", *temp_ptr);
        //  printf("\n[TRACE]: getSize size: %d\n", size);
        size++;
    }
    return size;
}

// FIXME: util or built in function for this?

void substringUntil(char *original, char *substr, int index)
{
    for (int i = 0; i <= index; i++)
    {
    }
}

enum HttpMethods
extractHttpMethod(char *buffer, size_t size)
{
    char *buffer_substr;
    buffer_substr = malloc(sizeof(char) * size);
    if (buffer_substr != NULL)
    {
        (void)substringUntil(buffer, buffer_substr, size / sizeof(char));
        enum HttpMethods method = HttpStrToMethod(buffer_substr);
        free(buffer_substr);
        return method;
    }
    return HttpFAKER;
}

enum HttpMethods
ParseRequestMethod(char *buffer, size_t buffer_length)
{
    printf("[TRACE]: entering ParseRequestMethod\n");
    size_t size = getMethodStrSize(buffer, buffer_length);
    // TODO TODO TODO

    // printf("[TRACE]: size is:  %d\n", size);
    if (size == 0)
    {
        return HttpFAKER;
    }
    return extractHttpMethod(buffer, size);
}

HttpRequest *CreateHttpRequest()
{
    return NULL;
}
