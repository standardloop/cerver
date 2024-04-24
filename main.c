#include <stdio.h>
#include <stdlib.h>

#include "./src/lib/cerver.h"
#include "./src/lib/logger.h"
#include "./src/lib/util/util.h"

void foo(HttpRequest *req, HttpResponse *res)
{
    // (void)Log(TRACE, "[JOSH]: entering special test function\n");
    char *accepted_types = MapGet(req->headers, "Accept");
    if (accepted_types == NULL)
    {
    }
    if (req == NULL || res == NULL)
    {
        return;
    }
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
    (void)StartCerver(server);

    return EXIT_SUCCESS;
}
