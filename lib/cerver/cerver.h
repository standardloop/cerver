#ifndef CERVER_H
#define CERVER_H
#include <netinet/in.h> // sockaddr_in

#define BUFFER_SIZE 1024

typedef struct
{
    int server_fd;
    struct sockaddr_in address;
    int addrlen;
    /*
    char * host;
    int port;
    // other settings
    */
} HTTPCerver;

HTTPCerver *Cerver(int);
void FreeCerver(HTTPCerver *);
void CerverStart(HTTPCerver *);

#endif
