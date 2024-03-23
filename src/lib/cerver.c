#include <stdio.h>      // perror
#include <sys/socket.h> // socket() bind() listen() accept() sockaddr
#include <stdlib.h>     // EXIT_FAILURE
#include <string.h>     // strlen()
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // write() close()
#include <string.h>
#include <pthread.h>

#include "cerver.h"
#include "./util/util.h"
#include "./thread/scheduler.h"

HTTPCerver *InitCerver(int port, int num_threads, int queue_buffer_size)
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

    HTTPCerver *cerver = (HTTPCerver *)malloc(sizeof(HTTPCerver));
    if (cerver == NULL)
    {
        perror("cannot allocate memory for cerver");
        exit(EXIT_FAILURE);
    }
    cerver->server_fd = server_fd;
    cerver->address = address;
    cerver->addrlen = addrlen;
    cerver->num_threads = num_threads;
    cerver->queue_buffer_size = queue_buffer_size;

    // if (router != NULL)
    // {
    // }
    return cerver;
}

void StartCerver(HTTPCerver *cerver)
{
    int new_socket;
    Scheduler *scheduler = InitScheduler(FIFO, cerver->queue_buffer_size);
    if (scheduler == NULL)
    {
        printf("[FATAL][5XX]: Couldn't allocate memory to scheduler");
        exit(EXIT_FAILURE);
    }

    ThreadPool *thread_pool = InitThreadPool(cerver->num_threads);
    if (thread_pool == NULL)
    {
        printf("[FATAL][5XX]: Couldn't allocate memory to thread_pool");
        exit(EXIT_FAILURE);
    }
    StartThreads(scheduler, thread_pool);

    while (FOREVER)
    {
        if ((new_socket = accept(cerver->server_fd, (struct sockaddr *)&cerver->address, (socklen_t *)&cerver->addrlen)) < 0)
        {
            printf("[FATAL]: Couldn't accept connections on socket?");
            exit(EXIT_FAILURE);
        }
        ScheduleRequestToBeHandled(scheduler, thread_pool, new_socket);
    }
    FreeThreadPool(thread_pool);
    FreeScheduler(scheduler);
}

// // FIXME
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
