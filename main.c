// Server side C program to demonstrate HTTP Server programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "./lib/cerver/cerver.h"

int main(int argc, char const *argv[])
{

    int port = 8080;

    int status = Cerver(port);
    return (status);
}
