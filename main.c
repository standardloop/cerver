#include <stdio.h>
#include <stdlib.h>

#include "./src/lib/cerver.h"

// int main(int argc, char const *argv[])
int main()
{
    /*
        parse argc/argv and environment
        char *my_env_var = getenv("PORT");
    */
    int port = 8080;
    int num_threads = 10;
    int buffer_size = 100;

    HTTPCerver *server = CerverInit(port, num_threads, buffer_size);
    CerverStart(server);
    /*

        Setup Paths here
    */
    // char *path = "/hello";

    // //printf("\nStarting Cerver on Port %d\n\n\n", port);
    return EXIT_SUCCESS;
}
