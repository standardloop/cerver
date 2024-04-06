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
#include "./response/codes.h"
#include "./../logger.h"

// FIXME — source of truth needs to be in "./response/codes/codes.h"
const char *BAD_GATEWAY_STRING = "HTTP/1.1 503 Bad Gateway\nContent-Type: text/plain\nContent-Length: 3\n\n503";
const char *NOT_FOUND_STRING = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 3\n\n404";
const char *BAD_REQUEST_STRING = "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\nContent-Length: 3\n\n400";
const char *STRING_405 = "HTTP/1.1 405 Method Not Allowed \nContent-Type: text/plain\nContent-Length: 3\n\n405";

void handleGenericError(int, enum HttpCode);

void HandleRequest(RouteTableAll *router, int client_socket)
{
    HttpRequest *request = NULL;
    HttpResponse *response = NULL;
    ssize_t valread;
    char buffer[BUFFER_SIZE];
    valread = read(client_socket, buffer, sizeof(char) * BUFFER_SIZE);
    if (valread == 0)
    {
        (void)Log(WARN, "[4XX]: Didn't read more than 0\n");
        handleGenericError(client_socket, HttpBadRequest);
    }
    else
    {
        request = ParseHttpRequest(buffer, valread); // pass valread here?
        if (request == NULL || request->early_resp_code != 0)
        {
            (void)handleGenericError(client_socket, request->early_resp_code);
        }
        else
        {
            if (router != NULL)
            {
                //(void)Log(TRACE, "Router is not NULL\n");
                response = (HttpResponse *)malloc(sizeof(HttpResponse));
                if (response == NULL)
                {
                    (void)Log(ERROR, "cannot allocate memory for HttpResponse\n");
                    (void)handleGenericError(client_socket, HttpBadGateway);
                }
                else
                {
                    switch (request->method)
                    {
                        // FIXME: treating HEAD and GET the same
                    case HttpHEAD:
                    case HttpGET:
                        if (router->get == NULL)
                        {
                            //(void)Log(TRACE, "[JOSH]: get router not found\n");
                            (void)handleGenericError(client_socket, HttpMethodNotAllowed);
                        }
                        else
                        {
                            // (void)PrintRouteTable(router->get);
                            Route *route = GetRouteFromTable(router->get, request->path);
                            if (route == NULL)
                            {
                                (void)Log(WARN, "404\n");
                                (void)handleGenericError(client_socket, HttpNotFound);
                            }
                            else
                            {
                                route->handler(request, response);
                                const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 6\n\nHello!";
                                (void)write(client_socket, hello, strlen(hello));
                            }
                        }
                        break;
                    case HttpOPTIONS:
                        if (router->options == NULL)
                        {
                            (void)handleGenericError(client_socket, HttpMethodNotAllowed);
                        }
                        break;
                    case HttpPOST:
                        if (router->post == NULL)
                        {
                            (void)handleGenericError(client_socket, HttpMethodNotAllowed);
                        }
                        break;
                    case HttpPUT:
                        if (router->put == NULL)
                        {
                            (void)handleGenericError(client_socket, HttpMethodNotAllowed);
                        }
                        break;
                    case HttpPATCH:
                        if (router->patch == NULL)
                        {
                            (void)handleGenericError(client_socket, HttpMethodNotAllowed);
                        }
                        break;
                    case HttpDELETE:
                        if (router->delete == NULL)
                        {
                            (void)handleGenericError(client_socket, HttpMethodNotAllowed);
                        }
                        break;
                    case HttpCONNECT:
                        if (router->delete == NULL)
                        {
                            (void)handleGenericError(client_socket, HttpMethodNotAllowed);
                        }
                        break;
                    case HttpTRACE:
                        if (router->trace == NULL)
                        {
                            (void)handleGenericError(client_socket, HttpMethodNotAllowed);
                        }
                        break;
                    case HttpFAKER:
                    default:
                        (void)Log(INFO, "HttpFAKER\n");
                        (void)handleGenericError(client_socket, HttpBadGateway);
                    }
                }
            }
        }
    }
    (void)FreeHttpResponse(response);
    (void)FreeHttpRequest(request);
    close(client_socket);
}

void handleStaticPath()
{
}

void handleGenericError(int client_socket, enum HttpCode response_code)
{
    switch (response_code)
    {
    case HttpMethodNotAllowed:
        (void)write(client_socket, STRING_405, strlen(STRING_405));
        break;
    case HttpNotFound:
        (void)write(client_socket, NOT_FOUND_STRING, strlen(NOT_FOUND_STRING));
        break;
    case HttpBadGateway:
        (void)write(client_socket, BAD_GATEWAY_STRING, strlen(BAD_GATEWAY_STRING));
        break;
    case HttpBadRequest:
        (void)write(client_socket, BAD_REQUEST_STRING, strlen(BAD_REQUEST_STRING));
        break;
    default:
        break;
    }
}
