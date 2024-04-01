#include <stdio.h>
#include <stdlib.h>

#include "./src/lib/cerver.h"

#include "./src/lib/logger.h"

// int main(int argc, char const *argv[])

void foo(HttpRequest *req, HttpResponse *res)
{
    if (req == NULL || res == NULL)
    {
        return;
    }
    (void)Log(TRACE, "[JOSH]: entering special test function\n");
    return;
}

int main(void)
{
    int port = 8080;
    int num_threads = 10;
    int buffer_size = 100;
    (void)SetLogLevel(TRACE);

    HTTPCerver *server = InitCerver(port, num_threads, buffer_size);

    int route_count = AddRouteToTable(server->router->get, "/foo", (RouterFunction *)foo);
    if (route_count < 0)
    {
        printf("\n%d\n", route_count);
        return EXIT_FAILURE;
    }
    (void)StartCerver(server);
    return EXIT_SUCCESS;
}
