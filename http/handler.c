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
#include <standardloop/logger.h>

// FIXME — source of truth needs to be in "./response/codes/codes.h"
const char *BAD_GATEWAY_STRING = "HTTP/1.1 503 Bad Gateway\nContent-Type: text/plain\nContent-Length: 3\n\n503";
const char *NOT_FOUND_STRING = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 3\n\n404";
const char *BAD_REQUEST_STRING = "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\nContent-Length: 3\n\n400";
const char *METHOD_NOT_SUPP_STRING = "HTTP/1.1 405 Method Not Allowed \nContent-Type: text/plain\nContent-Length: 3\n\n405";

// Looks of work needed here
void SendResponse(HttpResponse *resp)
{
    if (resp == NULL)
    {
        // FIXME: handle generic error
        HandleGenericError(resp->client_socket, HttpBadGateway);
        return;
    }
    // add Content-Length header
    HashMapInsert(resp->headers, JSONValueInit(STRING_t, Int64ToString(strlen(resp->body)), QuickAllocatedString("Content-Length")));

    // <VERSION> <STATUS_CODE> <STATUS_STRING>\n<HEADERS>\n\n<BODY>

    char *response_as_string = HttpResponseToString(resp); // maybe send length back so don't have to compute
    if (response_as_string == NULL)
    {
        HandleGenericError(resp->client_socket, HttpBadGateway);
        return;
    }
    (void)write(resp->client_socket, response_as_string, strlen(response_as_string));
    // const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 6\n\nHello!";
    // (void)write(resp->client_socket, hello, strlen(hello));
}

void HandleRequest(Router *router, int client_socket)
{
    HttpRequest *request = NULL;
    HttpResponse *response = NULL;
    ssize_t valread;
    char buffer[BUFFER_SIZE];
    valread = read(client_socket, buffer, sizeof(char) * BUFFER_SIZE);
    if (valread == 0)
    {
        Log(WARN, "[4XX]: Didn't read more than 0\n");
        HandleGenericError(client_socket, HttpBadRequest);
    }
    else
    {
        request = CreateParsedHttpRequest(buffer, valread);
        if (request == NULL || request->bail_resp_code != 0)
        {
            HandleGenericError(client_socket, request->bail_resp_code);
        }
        else
        {
            request->client_socket = client_socket; // does req obj need this information?
            if (router != NULL)
            {
                Log(TRACE, "Router is not NULL");
                response = CreateHttpResponse();
                if (response == NULL)
                {
                    Log(ERROR, "cannot allocate memory for HttpResponse\n");
                    HandleGenericError(client_socket, HttpBadGateway);
                }
                else
                {
                    response->client_socket = client_socket;
                    response->version = request->version;
                    Route *route = NULL;
                    bool router_found = false;
                    switch (request->method)
                    {
                    // FIXME: treating HEAD and GET the same
                    case HttpHEAD:
                    case HttpGET:
                        if (router->get != NULL)
                        {
                            router_found = true;
                            route = GetRouteFromTable(router->get, request->path);
                        }
                        break;
                    case HttpOPTIONS:
                        break;
                    case HttpPOST:
                        break;
                    case HttpPUT:
                        break;
                    case HttpPATCH:
                        break;
                    case HttpDELETE:
                        break;
                    case HttpCONNECT:
                        break;
                    case HttpTRACE:
                        break;
                    case HttpFAKE:
                    default:
                        Log(INFO, "HttpFAKE\n");
                        break;
                    }
                    if (!router_found)
                    {
                        Log(WARN, "405");
                        HandleGenericError(client_socket, HttpMethodNotAllowed);
                    }
                    else if (route == NULL)
                    {
                        Log(WARN, "404");
                        HandleGenericError(client_socket, HttpNotFound);
                    }
                    else
                    {
                        // We only know about path/route params after we have identified the route
                        // We cannot parse this CreateParsedHttpRequest because there isn't enough information
                        if (route->params != NULL)
                        {
                            request->path_params = ParsePathParams(route->params);
                        }
                        route->handler(request, response); // all handler functions should read the request object and modify the respone obj
                        // handler should use HttpResponseSend();
                    }
                }
            }
        }
    }
    FreeHttpRequest(request);
    FreeHttpResponse(response);
    close(client_socket);
}

void HandleGenericError(int client_socket, enum HttpCode response_code)
{
    switch (response_code)
    {
    case HttpMethodNotAllowed:
        (void)write(client_socket, METHOD_NOT_SUPP_STRING, strlen(METHOD_NOT_SUPP_STRING));
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
        (void)write(client_socket, METHOD_NOT_SUPP_STRING, strlen(METHOD_NOT_SUPP_STRING));
        break;
    }
}

void HandleStaticPath(int client_socket, char *path)
{
    Log(TRACE, "[handleStaticPath]");

    FILE *file;
    file = fopen(path + 1, "r");
    if (file == NULL)
    {
        HandleGenericError(client_socket, HttpNotFound);
        return;
    }
    size_t bytes_read;
    char buffer[BUFFER_SIZE];
    snprintf(buffer, strlen("HTTP/1.1 200 OK\r\n\r\n"), "HTTP/1.1 200 OK\r\n\r\n"); // FIXME, clean thi sup
    write(client_socket, buffer, strlen(buffer));

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0)
    {
        write(client_socket, buffer, bytes_read);
    }
    fclose(file);
}
