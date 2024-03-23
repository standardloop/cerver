#include <stdio.h>      // perror
#include <sys/socket.h> // socket() bind() listen() accept() sockaddr
#include <stdlib.h>     // EXIT_FAILURE
#include <string.h>     // strlen()
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // write() close()
#include <string.h>
#include <pthread.h>

#include "cerver.h"
#include "./http/request/request.h"
#include "./util/util.h"

void *handleRequest(void *);

HTTPCerver *Cerver(int port)
{
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    // TODO: re-use options for this
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    HTTPCerver *cerver = malloc(sizeof(HTTPCerver *));
    if (cerver == NULL)
    {
        perror("cannot allocate memory for cerver");
        exit(EXIT_FAILURE);
    }
    cerver->server_fd = server_fd;
    cerver->address = address;
    cerver->addrlen = addrlen;
    return cerver;
}

void CerverStart(HTTPCerver *cerver)
{
    // int num_threads = 10;

    int new_socket;
    while (1)
    {
        if ((new_socket = accept(cerver->server_fd, (struct sockaddr *)&cerver->address, (socklen_t *)&cerver->addrlen)) < 0)
        {
            printf("[FATAL]: Couldn't accept connections on socket?");
            exit(EXIT_FAILURE);
        }
        // pthread_t *t = malloc(sizeof(pthread_t));
        // if (t == NULL)
        // {
        //     printf();
        // }
        pthread_t t;
        int *pClient = malloc(sizeof(int));
        if (pClient == NULL)
        {
            printf("\n[ERROR][5XX]: couldn't secure memory for socker a connection");
            fflush(stdout);
        }
        *pClient = new_socket;
        printf("\naboout to call pthread create\n");
        fflush(stdout);
        pthread_create(&t, NULL, handleRequest, pClient);
        //  handleRequest(new_socket);
        // handleRequest(pClient);

        // close(new_socket);
    }
}

void *handleRequest(void *p_new_socket)
{
    printf("\ninside handleRequest\n");
    fflush(stdout);
    int client_socket = *(int *)p_new_socket;
    free(p_new_socket); // dont need anymore
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    ssize_t valread;
    char buffer[BUFFER_SIZE];
    // char *buffer = malloc(sizeof(char) * buffer_size); // no malloc here?
    valread = read(client_socket, buffer, sizeof(char) * BUFFER_SIZE);
    if (valread == 0)
    {
        printf("\n[FATAL]: Didn't read more than 0\n");
        // free(buffer);
        close(client_socket);
        return NULL;
        // exit(EXIT_FAILURE);
    }

    HttpRequest *request = CreateHttpRequest(buffer, BUFFER_SIZE); // pass valread here?
    PrintHttpRequest(request);
    if (request == NULL)
    {
        // if you cannot parse the request, we need to return a 4XX?
        // free(buffer);
        printf("\n[ERROR][4|5XX]: HttpRequest fail to parse\n");

        // exit(EXIT_FAILURE);
    }
    else
    {
        // create response
        //
        write(client_socket, hello, strlen(hello));
    }
    // free(buffer);
    FreeHttpRequest(request);
    close(client_socket);
    return NULL;
}

// // FIXME cheater
// void handle_request(int client_socket)
// {
//     char buffer[BUFFER_SIZE];
//     char filename[BUFFER_SIZE];
//     FILE *file;
//     int bytes_read;

//     bytes_read = read(client_socket, buffer, BUFFER_SIZE);
//     if (bytes_read < 0)
//     {
//         perror("fai");
//         exit(EXIT_FAILURE);
//     }

//     sscanf(buffer, "GET %s HTTP/1.1\r\n", filename);
//     file = fopen(filename + 1, "r");
//     if (file == NULL)
//     {
//         // 404
//     }
//     sprintf(buffer, "HTTP/1.1 200 OK\r\n\r\n");
//     write(client_socket, buffer, strlen(buffer));

//     while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0)
//     {
//         write(client_socket, buffer, bytes_read);
//     }
//     fclose(file);
// }
