#include <stdio.h>
#include <stdlib.h>

#include "./lib/cerver/cerver.h"

// int main(int argc, char const *argv[])
int main()
{

    /*
        parse argc/argv and environment
        char *my_env_var = getenv("PORT");
    */

    int port = 8080;

    /*
        Setup Threads
    */
    HTTPCerver *server = Cerver(port);
    CerverStart(server);

    /*
        
        Setup Paths here
    */
    // char *path = "/hello";

    // //printf("\nStarting Cerver on Port %d\n\n\n", port);
    return EXIT_SUCCESS;
}
