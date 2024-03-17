#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "request.h"
#include <stdbool.h>
#include "../methods/methods.h"
#include "../version/version.h"
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
    while (temp_ptr != NULL && *temp_ptr != '\0' && *temp_ptr != '\n' && char_count < buffer_size)
    {

        temp_ptr++;
        char_count++;
    }
    temp_ptr = NULL;
    return char_count;
}

HttpRequest *CreateHttpRequest(char *buffer, size_t buffer_size)
{

    //(void)PrintBuffer(buffer);

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
    size_t moved = howMuchToMoveToNewLine(buffer, buffer_size);
    //(void)PrintBuffer(buffer);

    request->method = ParseRequestMethod(buffer, moved);
    if (request->method == HttpFAKER)
    {
        free(request);
        return NULL;
    }

    request->http_verion = ParseHttpVersion(buffer, moved);
    // printf("\nVersion: %.1f", request->http_verion);
    if (request->http_verion == 0.0)
    {
        free(request);
        return NULL;
    }
    // FIXME: dead memory
    buffer += moved;
    moved = howMuchToMoveToNewLine(buffer, buffer_size);
    request->host = NULL;
    request->port = 8080; // FIXME
    request->headers = NULL;
    request->body = NULL;
    return request;
}

void FreeHttpRequest(HttpRequest *request)
{
    // free other sections first
    free(request);
    return;
}
