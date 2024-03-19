#include <stdio.h>
#include <stdlib.h>

#include "./lib/cerver/cerver.h"

// int main(int argc, char const *argv[])
int main()
{

    // FIXME: more logically start up (also look into the arg parser for fun)
    // char *my_env_var = getenv("PORT");

    int port = 8080;
    HTTPCerver *server = Cerver(port);
    CerverStart(server);
    //char *path = "/hello";

    // //printf("\nStarting Cerver on Port %d\n\n\n", port);
    return EXIT_SUCCESS;
}
