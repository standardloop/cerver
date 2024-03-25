#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <sys/socket.h> // socket() bind() listen() accept() sockaddr
#include <stdlib.h>     // EXIT_FAILURE
#include <string.h>     // strlen()
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // write() close()
#include <pthread.h>
#include <stdbool.h>

#include "./request.h"

#include "./parser/method.h"
#include "./parser/path.h"

#include "./version/version.h"
#include "./host/host.h"
#include "./port/port.h"
#include "../../util/util.h"

#include "../../logger.h"

// HEAD / HTTP/1.1
// Host: localhost:8080
// User-Agent: curl/7.79.1
// Accept: */*

void HandleRequest(int client_socket)
{

    ssize_t valread;
    char buffer[BUFFER_SIZE];
    valread = read(client_socket, buffer, sizeof(char) * BUFFER_SIZE);
    if (valread == 0)
    {
        (void)Log(ERROR, "[4XX]: Didn't read more than 0\n");
    }
    else
    {
        // printf(c, buffer);//DELETE
        HttpRequest *request = ParseHttpRequest(buffer, valread); // pass valread here?
        if (request == NULL)
        {
            (void)Log(ERROR, "[4|5XX]:HttpRequest fail to parse\n");
        }
        // FIXME ELSE
        char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
        write(client_socket, hello, strlen(hello));
        FreeHttpRequest(request);
    }
    close(client_socket);
}

HttpRequest *ParseHttpRequest(char *buffer, size_t buffer_size)
{
    // return NULL; // prove if server will crash or not XD
    if (buffer == NULL || buffer_size == 0)
    {
        (void)Log(ERROR, "[4XX]: buffer is NULL for ParseHttpRequest or buffer_size is 0\n");
        return NULL;
    }
    HttpRequest *request = malloc(sizeof(HttpRequest));
    if (request == NULL)
    {
        (void)Log(ERROR, "[5XX]: buffer is NULL for malloc in ParseHttpRequest\n");
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

    char *buffer_start = buffer;
    char *space_pointer = strchr(buffer, SPACE_CHAR);
    if (*space_pointer != SPACE_CHAR)
    {
        Log(WARN, "[4XX]: space_point is not a space!!!!!\n");
        return request;
    }
    size_t suspected_http_method_length = (space_pointer - buffer_start);

    request->method = ParseRequestMethod(buffer_start, suspected_http_method_length);
    if (request->method == HttpFAKER)
    {
        Log(WARN, "[4XX]: Couldn't parser HTTP Request method\n");
        return request;
    }

    buffer = buffer_start;
    char *space_pointer_again = strchr((space_pointer + 1), SPACE_CHAR); // +1 because pointer is on space
    if ((*space_pointer_again != SPACE_CHAR) && (*space_pointer != SPACE_CHAR))
    {
        (void)Log(WARN, "error finding appropriate amount of spaces for path+query");
    }
    size_t suspected_path_length = (space_pointer_again - space_pointer);

    char *path = ParseRequestPath(space_pointer + 1, suspected_path_length);
    request->path_and_query = path;
    if (request->path_and_query == NULL)
    {
        Log(WARN, "[4XX]: Couldn't parser HTTP Request path/query\n");
        return request;
    }
    return request;
}

void FreeHttpRequest(HttpRequest *request)
{
    if (request == NULL)
    {
        return;
    }
    if (request->path_and_query != NULL)
    {
        free(request->path_and_query);
    }
    if (request->host != NULL)
    {
        free(request->host);
    }
    free(request);
}

// FIXME user logger here
void PrintHttpRequest(HttpRequest *request)
{
    if (request->method != HttpFAKER)
    {
        printf("[DEBUG][HTTPMETHOD]: %s\n", HttpMethodToStr(request->method));
    }
    if (request->version != ERROR_FLOAT)
    {
        printf("[DEBUG][HTTPVERSION]: %.1f\n", request->version);
    }
    if (request->host != NULL)
    {
        printf("[DEBUG][HTTPHOST]: %s\n", request->host);
    }
    if (request->port != 0)
    {
        printf("[DEBUG][HTTPPORT]: %d\n", request->port);
    }
}
