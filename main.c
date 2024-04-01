#include <stdio.h>
#include <stdlib.h>

#include "./src/lib/cerver.h"

#include "./src/lib/logger.h"

// int main(int argc, char const *argv[])

void test(HttpRequest *req, HttpResponse *res);
void test(HttpRequest *req, HttpResponse *res)
{
    if (req == NULL || res == NULL)
    {
        return;
    }
    return;
}

int main(void)
{
    int port = 8080;
    int num_threads = 10;
    int buffer_size = 100;
    (void)SetLogLevel(WARN);

    HTTPCerver *server = InitCerver(port, num_threads, buffer_size);
    // AddPathCerver

    int foo = AddRouteToTable(server->get_route_table, HttpGET, "/foo", test);
    (void)StartCerver(server);
    return EXIT_SUCCESS;
}
