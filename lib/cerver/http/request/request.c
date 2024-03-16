#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "request.h"
#include <stdbool.h>
#include "../methods/methods.h"
#include "../version/version.h"

// HEAD / HTTP/1.1
// Host: localhost:8080
// User-Agent: curl/7.79.1
// Accept: */*

// size_t getMethodStrSize(char *, size_t);

// Long Term FIXME but get working server first
// Free Memory of parts of buffer
// Paralleize this code
HttpRequest *CreateHttpRequest(char *buffer, size_t buffer_size)
{
    if (buffer == NULL)
    {
        return NULL;
    }
    // WIP
    HttpRequest *request = malloc(sizeof(CreateHttpRequest));
    if (request == NULL)
    {
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
    char *buffer_ptr = buffer;
    size_t char_count = 0;
    int line_num = 0;
    while (buffer_ptr != NULL && *buffer_ptr != '\0' && char_count < buffer_size && line_num < MAX_LINE_LENGTH)
    {
        if (line_num == 1)
        {
            // pass char count to only allow the function to access part of the string
            request->method = ParseRequestMethod(buffer, buffer_size);
            if (request->method == HttpFAKER)
            {
                free(request);
                return NULL;
            }
            printf("\nMethod: %d", request->method);
            request->httpVerion = ParseHttpVersion(buffer, buffer_size);
            printf("\nVersion: %f", request->httpVerion);
            if (request->httpVerion == 0.0)
            {
                free(request);
                return NULL;
            }
        }
        // printf("%c", *buffer_ptr);
        if (*buffer_ptr == '\n')
        {
            line_num++;
        }

        buffer_ptr++;
        char_count++;
    }

    request->host = NULL;
    request->port = 8080; // FIXME
    request->headers = NULL;
    request->body = NULL;
    return request;
}

// bool FreeHttpRequest()
// {
//     return false;
// }
