#include <stdio.h>
#include <stdlib.h>

#include "./src/lib/cerver.h"
#include "./src/lib/logger.h"

void foo(HttpRequest *req, HttpResponse *res)
{
    // (void)Log(TRACE, "[JOSH]: entering special test function\n");
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

    (void)SetLogLevel(TRACE);

    Cerver *server = InitCerver(port, num_threads, buffer_size);
    (void)AddRouteToTable(server->router->get, "/foo", (RouteHandler *)foo);
    (void)StartCerver(server);

    return EXIT_SUCCESS;
}
