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
    if (*temp_ptr == '\n')
    {
        // printf("\n[ERROR]: received a newline at start in howMuchToMoveToNewLine\n");
    }
    while (*temp_ptr != '\n')
    {
        // //printf("\n[JOSH]: temp_ptr in howMuchToMoveToNewLine is: %c\n", *temp_ptr);
        if (temp_ptr == NULL || *temp_ptr == '\0' || char_count >= buffer_size)
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
    // printf("\n[INFO]: entering CreateHttpRequest");
    //(void)PrintBuffer(buffer);
    char *buffer_start = buffer;
    int line_num = 0;
    size_t moved = 0;
    if (buffer == NULL)
    {
        // printf("\n[ERROR]: buffer is NULL for CreateHttpRequest\n");
        buffer = buffer_start;
        return NULL;
    }
    // WIP
    HttpRequest *request = malloc(sizeof(CreateHttpRequest));
    if (request == NULL)
    {
        // printf("\n[ERROR]: buffer is NULL for malloc in CreateHttpRequest\n");
        buffer = buffer_start;
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

    if (buffer == NULL || moved == 0)
    {
        FreeHttpRequest(request);
        buffer = buffer_start;
        // printf("\n[ERROR]: couldn't move to the next line in the request: line_num: %d\n", line_num);
        return NULL;
    }
    line_num++;
    request->method = ParseRequestMethod(buffer, moved);
    if (request->method == HttpFAKER)
    {
        FreeHttpRequest(request);
        buffer = buffer_start;
        // printf("\n[ERROR]: couldn't get a valid http reques method");
        return NULL;
    }

    request->http_verion = ParseHttpVersion(buffer, moved);

    if (request->http_verion == ERROR_FLOAT)
    {
        FreeHttpRequest(request);
        buffer = buffer_start;
        // printf("\n[ERROR]: couldn't get a valid http version\n");
        return NULL;
    }
    // //printf("\nVersion: %.1f", request->http_verion);
    // FIXME: dead memory
    buffer++; // '\n'
    buffer += moved;
    moved = howMuchToMoveToNewLine(buffer, buffer_size);

    if (buffer == NULL || moved == 0)
    {
        FreeHttpRequest(request);
        buffer = buffer_start;
        printf("\n[ERROR]: couldn't move to the next line in the request: line_num: %d\n", line_num);
        return NULL;
    }
    // PrintBuffer(buffer);
    request->host = ParseHost(buffer, moved);
    if (request->host == NULL)
    {
        buffer = buffer_start;
        printf("\n[ERROR][4XX]: couldn't get a valid host\n");
        FreeHttpRequest(request);
        return NULL;
    }
    request->port = ParsePort(buffer, moved);
    if (request->port == ERROR_PORT)
    {
        buffer = buffer_start;
        FreeHttpRequest(request);
        printf("\n[ERROR][4XX]: couldn't get a valid port\n");
        return NULL;
    }
    printf("\n[INFO][2XX]: port: %d\n", request->port);

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
    if (request->host != NULL)
    {
        free(request->host);
    }
    free(request);
    return;
}
