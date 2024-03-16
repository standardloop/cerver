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
    HttpRequest *request = malloc(sizeof(CreateHttpRequest));
    if (request == NULL)
    {
        return NULL;
    }
    request->method = ParseRequestMethod(buffer, buffer_size);
    if (request->method == HttpFAKER)
    {
        free(request);
        return NULL;
    }
    // FIXME Free memory from the this since we already got what we want
    request->httpVerion = ParseHttpVersion(buffer, buffer_size);
    if (request->httpVerion == 0.0)
    {
        free(request);
        return NULL;
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
