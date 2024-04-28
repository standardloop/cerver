#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "./src/lib/cerver.h"
#include "./src/lib/http/handler.h"
#include "./src/lib/logger.h"
#include "./src/lib/util/util.h"

void foo(HttpRequest *, HttpResponse *);
void fooID(HttpRequest *, HttpResponse *);
void fooStatic(HttpRequest *, HttpResponse *);

void foo(HttpRequest *request, HttpResponse *response)
{
    // (void)Log(TRACE, "[JOSH]: entering special test function\n");
    // char *accepted_types = MapGet(request->headers, "Accept");
    // if (accepted_types == NULL)
    // {
    // }
    if (request == NULL || response == NULL)
    {
        return;
    }
    const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 6\n\nHello!";
    (void)write(request->client_socket, hello, strlen(hello));
}

// "/foo/:id"
void fooID(HttpRequest *request, HttpResponse *response)
{
    // char * path_param_id = GetPathParam(request->path_params, "id");
    (void)Log(TRACE, "[JOSH]: entering special test ID function\n");
    if (request == NULL || response == NULL)
    {
        return;
    }
    const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 6\n\nHello!";
    (void)write(request->client_socket, hello, strlen(hello));
}

void fooStatic(HttpRequest *request, HttpResponse *response)
{
    // (void)Log(TRACE, "[JOSH]: entering special test function\n");
    char *accepted_types = MapGet(request->headers, "Accept");
    if (accepted_types == NULL)
    {
    }
    if (request == NULL || response == NULL)
    {
        return;
    }
    HandleStaticPath(request->client_socket, "/static/foo.html");
    return;
}

// int main(int argc, char const *argv[])
int main(void)
{
    int port = 8080;
    int num_threads = 10;
    int buffer_size = 100;

    const char *log_level_str = GetEnv("LOG_LEVEL", "TRACE");
    enum LogLevel log_level_enum = StringToLogLevel(log_level_str);
    (void)SetLogLevel(log_level_enum);

    Cerver *server = InitCerver(port, num_threads, buffer_size);

    // You should now at compile time how many routes you have
    // int num_routes = 1;

    (void)AddRouteToTable(server->router->get, "/foo", (RouteHandler *)foo);

    // WIP parse this (path params) /foo/4 (match) /foo/bar (no match)
    (void)AddRouteToTable(server->router->get, "/foo/{id=int}", (RouteHandler *)fooID);
    // WIP parse wildcard as well
    (void)AddRouteToTable(server->router->get, "/foo/*", (RouteHandler *)foo);

    (void)AddRouteToTable(server->router->get, "/foo/bar", (RouteHandler *)foo); // WIP parse this (no params)
    (void)AddRouteToTable(server->router->get, "/static/foo.html", (RouteHandler *)fooStatic);

    //(void)AddRouteToTable(server->router->get, "/client/{clientID}/department/{departmentID}/employees", (RouteHandler *)foo); // WIP parse this (no params)
    //(void)AddRouteToTable(server->router->get, "/static/*", (RouteHandler *)fooStatic); // WIP parse this (wildcard)
    (void)StartCerver(server);

    return EXIT_SUCCESS;
}
