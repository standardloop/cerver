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

#include "./handler.h"
#include "./request/request.h"
#include "./response/codes/codes.h"
#include "./../logger.h"

// FIXME — source of truth needs to be in "./response/codes/codes.h"
const char *BAD_GATEWAY_STRING = "HTTP/1.1 503 Bad Gateway\nContent-Type: text/plain\nContent-Length: 3\n\n503";
const char *NOT_FOUND_STRING = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 3\n\n404";
const char *BAD_REQUEST_STRING = "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\nContent-Length: 3\n\n400";

const char *handleParserError(HttpRequest *);

void HandleRequest(int client_socket)
{
    ssize_t valread;
    char buffer[BUFFER_SIZE];
    valread = read(client_socket, buffer, sizeof(char) * BUFFER_SIZE);
    if (valread == 0)
    {
        (void)Log(ERROR, "[4XX]: Didn't read more than 0\n");
        write(client_socket, NOT_FOUND_STRING, strlen(NOT_FOUND_STRING));
    }
    else
    {
        HttpRequest *request = ParseHttpRequest(buffer, valread); // pass valread here?
        if (request == NULL || request->error != NULL)
        {
            const char *error_response_string = handleParserError(request);
            if (error_response_string == NULL)
            {
                (void)Log(ERROR, "[4|5XX]:HttpRequest fail to parse completely\n");
                write(client_socket, BAD_GATEWAY_STRING, strlen(BAD_GATEWAY_STRING));
            }
            write(client_socket, error_response_string, strlen(error_response_string));
            // free(error_response_string);
        }
        else
        {
            // Check if HTTP request is in a route table
            const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
            write(client_socket, hello, strlen(hello));
        }
        FreeHttpRequest(request);
    }
    close(client_socket);
}

const char *handleParserError(HttpRequest *request)
{
    if (request == NULL || request->error == NULL)
    {
        return NULL;
    }
    if (request->host)
    {
    }
    if (request->method == HttpFAKER)
    {
        return BAD_REQUEST_STRING;
    }
    if (request->path_and_query)
    {
    }
    if (request->port)
    {
    }
    if (request->version)
    {
    }
    return NULL;
}