#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "./src/lib/cerver.h"
#include "./src/lib/logger.h"
#include "./src/lib/util/util.h"

void handleStaticPath(int, char *);

void foo(HttpRequest *, HttpResponse *);
void fooStatic(HttpRequest *, HttpResponse *);

void foo(HttpRequest *request, HttpResponse *response)
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
    const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 6\n\nHello!";
    (void)write(request->client_socket, hello, strlen(hello));
}

void handleStaticPath(int client_socket, char *path)
{
    FILE *file;
    file = fopen(path + 1, "r");
    if (file == NULL)
    {
       // (void)handleGenericError(client_socket, HttpNotFound);
    }
    size_t bytes_read;
    char buffer[BUFFER_SIZE];
    sprintf(buffer, "HTTP/1.1 200 OK\r\n\r\n");
    write(client_socket, buffer, strlen(buffer));

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0)
    {
        write(client_socket, buffer, bytes_read);
    }
    fclose(file);
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
    handleStaticPath(request->client_socket, "/static/foo");
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
    (void)AddRouteToTable(server->router->get, "/static/foo.html", (RouteHandler *)fooStatic);
    (void)StartCerver(server);

    return EXIT_SUCCESS;
}
