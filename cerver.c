#include <stdio.h>
#include <sys/socket.h> // socket() bind() listen() accept() sockaddr
#include <stdlib.h>     // EXIT_FAILURE
#include <string.h>     // strlen()
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // write() close()
#include <string.h>
#include <pthread.h>

#include <standardloop/logger.h>
#include <standardloop/util.h>

#include "cerver.h"

#include "./thread/scheduler.h"

Cerver *InitCerver(int port, int num_threads, int queue_buffer_size)
{
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        Log(FATAL, "cannot create socker file descriptor\n");
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
    {
        Log(FATAL, "couldn't set socket options\n");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        Log(FATAL, "Couldnt bind socket");
    }
    if (listen(server_fd, 10) < 0)
    {
        Log(FATAL, "couldn't listen on socket\n");
    }

    Cerver *cerver = (Cerver *)malloc(sizeof(Cerver));
    if (cerver == NULL)
    {
        Log(FATAL, "cannot allocate memory for cerver\n");
    }
    cerver->server_fd = server_fd;
    cerver->address = address;
    cerver->addrlen = addrlen;
    cerver->num_threads = num_threads;
    cerver->queue_buffer_size = queue_buffer_size;

    cerver->router = InitRouter();
    if (cerver->router == NULL)
    {
        free(cerver);
        Log(FATAL, "cannot allocate memory for cerver\n");
        return NULL;
    }

    // if (router != NULL)
    // {
    // }

    return cerver;
}

void StartCerver(Cerver *cerver)
{
    int client_socket;
    Scheduler *scheduler = InitScheduler(FIFO, cerver->queue_buffer_size);
    if (scheduler == NULL)
    {
        Log(FATAL, "[5XX]: Couldn't allocate memory to scheduler\n");
    }
    Log(TRACE, "Starting scheduler");

    ThreadPool *thread_pool = InitThreadPool(cerver->num_threads);
    if (thread_pool == NULL)
    {
        Log(FATAL, "[5XX]: Couldn't allocate memory to thread_pool\n");
    }
    StartThreads(cerver->router, scheduler, thread_pool);
    Log(TRACE, "Starting threads");
    while (ALWAYS)
    {
        if ((client_socket = accept(cerver->server_fd, (struct sockaddr *)&cerver->address, (socklen_t *)&cerver->addrlen)) < 0)
        {
            Log(ERROR, "[5XX]: Couldn't accect connections on socket\n");
        }
        else
        {
            ScheduleRequestToBeHandled(scheduler, thread_pool, client_socket);
        }
    }
    FreeThreadPool(thread_pool);
    FreeScheduler(scheduler);
}
