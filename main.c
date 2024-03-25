#include <stdio.h>
#include <stdlib.h>

#include "./src/lib/cerver.h"
#include "./src/lib/logger.h"

// int main(int argc, char const *argv[])
int main()
{
    int port = 8080;
    int num_threads = 10;
    int buffer_size = 100;
    (void)SetLogLevel(FATAL);

    HTTPCerver *server = InitCerver(port, num_threads, buffer_size);
    // AddPathCerver
    (void)StartCerver(server);
    return EXIT_SUCCESS;
}
