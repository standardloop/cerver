#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "request.h"
#include <stdbool.h>
#include "./methods/methods.h"
#include "./version/version.h"
#include "./host/host.h"
#include "./port/port.h"
#include "../../util/util.h"

// HEAD / HTTP/1.1
// Host: localhost:8080
// User-Agent: curl/7.79.1
// Accept: */*

// size_t getMethodStrSize(char *, size_t);

// Long Term FIXME but get working server first
// Free Memory of parts of buffer
// Paralleize this code

size_t howMuchToMoveToNewLine(char *buffer, size_t buffer_size)
{
    size_t char_count = 0;
    char *temp_ptr = buffer;
    while (*temp_ptr != '\n')
    {
        if (temp_ptr == NULL || *temp_ptr == '\0' || char_count < buffer_size)
        {
            return ERROR_SIZE_T;
        }
        temp_ptr++;
        char_count++;
    }
    temp_ptr = NULL;
    return char_count;
}

HttpRequest *CreateHttpRequest(char *buffer, size_t buffer_size)
{

    //(void)PrintBuffer(buffer);
    char *buffer_start = buffer;
    size_t moved;

    if (buffer == NULL)
    {
        printf("\n[ERROR]: buffer is NULL for CreateHttpRequest\n");
        return NULL;
    }
    // WIP
    HttpRequest *request = malloc(sizeof(CreateHttpRequest));
    if (request == NULL)
    {
        printf("\n[ERROR]: buffer is NULL for malloc in CreateHttpRequest\n");
        return NULL;
    }
    /*
    GET / HTTP/1.1
    Host: localhost:8080
    User-Agent: curl/7.79.1
    Accept: wildcard
    Connection: Upgrade, HTTP2-Settings
    Upgrade: h2c
    HTTP2-Settings: AAMAAABkAAQCAAAAAAIAAAAA
    */

    // Run through buffer line by line
    moved = howMuchToMoveToNewLine(buffer, buffer_size);
    //(void)PrintBuffer(buffer);

    request->method = ParseRequestMethod(buffer, moved);
    if (request->method == HttpFAKER)
    {
        free(request);
        return NULL;
    }

    request->http_verion = ParseHttpVersion(buffer, moved);

    if (request->http_verion == ERROR_FLOAT)
    {
        free(request);
        return NULL;
    }
    // printf("\nVersion: %.1f", request->http_verion);
    // FIXME: dead memory
    buffer++; // '\n'
    buffer += moved;
    moved = howMuchToMoveToNewLine(buffer, buffer_size);

    if (buffer == NULL || moved == 0)
    {
        printf("\n[FATAL]: how is this is happening");
    }
    // PrintBuffer(buffer);
    request->host = ParseHost(buffer, moved);
    if (request->host == NULL)
    {
        buffer = buffer_start;
        free(request);
        return NULL;
    }
    request->port = 8080;
    // request->port = ParsePort(buffer, moved);
    // if (request->port == ERROR_PORT)
    // {
    //     buffer = buffer_start;
    //     free(request);
    //     return NULL;
    // }

    buffer += moved;
    moved = howMuchToMoveToNewLine(buffer, buffer_size);

    request->headers = NULL;
    request->body = NULL;

    // FIXME free line by line but for now just reset it
    buffer = buffer_start;
    return request;
}

void FreeHttpRequest(HttpRequest *request)
{
    // free other sections first
    free(request);
    return;
}
