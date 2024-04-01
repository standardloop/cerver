#ifndef CERVER_H
#define CERVER_H

#include <netinet/in.h> // sockaddr_in
#define SOCKET_ERROR -1 // WIP

#include "./http/router.h"

typedef struct
{
    int server_fd;
    struct sockaddr_in address;
    int addrlen;
    int num_threads;
    int queue_buffer_size;
    RouteTableAll *router;

    // FIXME do we need more?
    // RouteTable *_table;
} HTTPCerver;

/*
 */

HTTPCerver *InitCerver(int, int, int);
void FreeCerver(HTTPCerver *);
void StartCerver(HTTPCerver *);

#endif
