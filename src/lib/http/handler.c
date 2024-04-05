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
#include "./router.h"
#include "./request/request.h"
#include "./response/response.h"
#include "./../logger.h"

// FIXME — source of truth needs to be in "./response/codes/codes.h"
const char *BAD_GATEWAY_STRING = "HTTP/1.1 503 Bad Gateway\nContent-Type: text/plain\nContent-Length: 3\n\n503";
const char *NOT_FOUND_STRING = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 3\n\n404";
const char *BAD_REQUEST_STRING = "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\nContent-Length: 3\n\n400";
const char *BAD_REQUEST_METHOD_STRING = "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\nContent-Length: 3\n\n400";

const char *handleParserError(HttpRequest *);

void HandleRequest(RouteTableAll *router, int client_socket)
{
    HttpRequest *request = NULL;
    HttpResponse *response = NULL;
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
        request = ParseHttpRequest(buffer, valread); // pass valread here?
        if (request == NULL || request->error != NULL)
        {
            const char *error_response_string = handleParserError(request);
            if (error_response_string == NULL)
            {
                (void)Log(ERROR, "[4|5XX]:HttpRequest fail to parse completely\n");
                write(client_socket, BAD_GATEWAY_STRING, strlen(BAD_GATEWAY_STRING));
            }
            else
            {
                write(client_socket, error_response_string, strlen(error_response_string));
            }
        }
        else
        {
            if (router != NULL)
            {
                (void)Log(TRACE, "Router is not NULL\n");
                response = (HttpResponse *)malloc(sizeof(HttpResponse));
                if (response == NULL)
                {
                    (void)Log(ERROR, "cannot allocate memory for HttpResponse\n");
                    write(client_socket, BAD_GATEWAY_STRING, strlen(BAD_GATEWAY_STRING));
                }
                else
                {
                    switch (request->method)
                    {
                        // FIXME: treating HEAD and GET the same
                    case HttpHEAD:
                    case HttpGET:
                        if (router->get != NULL)
                        {
                            // (void)PrintRouteTable(router->get);
                            Route *route = GetRouteFromTable(router->get, request->path);
                            if (route != NULL)
                            {
                                route->handler(request, response);
                                const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 6\n\nHello!";
                                (void)write(client_socket, hello, strlen(hello)); // FIXME return code
                            }
                            else
                            {
                                (void)Log(WARN, "[JOSH]: 404\n");
                            }
                        }
                        else
                        {
                            (void)Log(WARN, "[JOSH]: get router not found\n");
                        }
                        break;
                    case HttpOPTIONS:
                    case HttpPOST:
                    case HttpPUT:
                    case HttpPATCH:
                    case HttpDELETE:
                    case HttpCONNECT:
                        write(client_socket, BAD_GATEWAY_STRING, strlen(BAD_REQUEST_METHOD_STRING));
                    case HttpTRACE:
                        write(client_socket, BAD_GATEWAY_STRING, strlen(BAD_REQUEST_METHOD_STRING));
                    case HttpFAKER:
                        (void)Log(INFO, "HttpFAKER\n");
                        write(client_socket, BAD_GATEWAY_STRING, strlen(BAD_REQUEST_METHOD_STRING));
                        break;
                    default:
                        return;
                    }
                }
            }
        }
    }
    (void)FreeHttpResponse(response);
    (void)FreeHttpRequest(request);
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
    if (request->path)
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

void handleStaticPath()
{
}
