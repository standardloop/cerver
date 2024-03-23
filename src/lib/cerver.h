#ifndef CERVER_H
#define CERVER_H
#include <netinet/in.h> // sockaddr_in

typedef struct
{
    int server_fd;
    struct sockaddr_in address;
    int addrlen;
    int num_threads;
    int queue_buffer_size;
    /*
    char * host;
    int port;
    // other settings
    */
} HTTPCerver;

HTTPCerver *CerverInit(int, int, int);
void FreeCerver(HTTPCerver *);
void CerverStart(HTTPCerver *);

#endif
