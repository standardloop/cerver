#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "./src/lib/cerver.h"
#include "./src/lib/http/handler.h"
#include "./src/lib/logger.h"
#include "./src/lib/util/util.h"

void foo(HttpRequest *, HttpResponse *);
void bar(HttpRequest *, HttpResponse *);
void fooID(HttpRequest *, HttpResponse *);
void fooStatic(HttpRequest *, HttpResponse *);

void foo(HttpRequest *request, HttpResponse *response)
{
    // Log(TRACE, "[JOSH]: entering special test function\n");
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

void bar(HttpRequest *request, HttpResponse *response)

{
    Log(TRACE, "[BAR]\n");
    // Log(TRACE, "[JOSH]: entering special test function\n");
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

void fooID(HttpRequest *request, HttpResponse *response)
{
    // char *path_param_id = GetPathParam(request->path_params, "id");
    char *path_param_id = MapGet(request->path_params, "id");
    if (path_param_id != NULL)
    {
        printf("\npath_param_id: %s\n", path_param_id);
    }

    Log(TRACE, "[JOSH]: entering special test ID function\n");
    if (request == NULL || response == NULL)
    {
        return;
    }
    const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 6\n\nHello!";
    (void)write(request->client_socket, hello, strlen(hello));
}

void fooStatic(HttpRequest *request, HttpResponse *response)
{
    // Log(TRACE, "[JOSH]: entering special test function\n");
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
    int port = atoi(GetEnv("PORT", "8080"));
    int num_threads = atoi(GetEnv("NUM_THREADS", "10"));
    int buffer_size = atoi(GetEnv("BUFFER_SIZE", "100"));

    SetLogLevel(StringToLogLevel(GetEnv("LOG_LEVEL", "TRACE")));

    Cerver *server = InitCerver(port, num_threads, buffer_size);

    (void)AddRouteToTable(server->router->get, "/foo", foo);
    (void)AddRouteToTable(server->router->get, "/foo/{id=int}/bar/{name=string}", fooID);

    // WIP support *
    //(void)AddRouteToTable(server->router->get, "/bar/.*", bar);

    StartCerver(server);

    return EXIT_SUCCESS;
}
