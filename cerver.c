#include <stdio.h>
#include <sys/socket.h> // socket() bind() listen() accept() sockaddr
#include <stdlib.h>     // EXIT_FAILURE
#include <string.h>     // strlen()
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // write() close()
#include <signal.h>     // signal polling for graceful shutdown
#include <errno.h>
#include <string.h>
#include <pthread.h>

#include <standardloop/logger.h>
#include <standardloop/util.h>

#include "cerver.h"

#include "./thread/scheduler.h"

volatile sig_atomic_t cerver_running = true;

void cerverSigHandler(int);

// NOTE: SIGKILL cannot be trapped
void cerverSigHandler(int signum)
{
    switch (signum)
    {
    case SIGINT:
        Log(ERROR, "SIGINT received!");
        break;
    case SIGTERM:
        Log(ERROR, "SIGTERM received!");
        break;
    default:
        break;
    }
    cerver_running = false;
}

Cerver *InitCerver(int port, int num_threads, int queue_buffer_size)
{
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        Log(FATAL, "cannot create socker file descriptor");
        return NULL;
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
    {
        Log(FATAL, "couldn't set socket options");
        return NULL;
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
    Log(INFO, "Starting Cerver on Port %d", port);
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
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = cerverSigHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        Log(FATAL, "sigaction SIGINT fail");
        return;
    }
    if (sigaction(SIGTERM, &sa, NULL) == -1)
    {
        Log(FATAL, "sigaction SIGTERM fail");
        return;
    }

    int client_socket;
    Scheduler *scheduler = InitScheduler(FIFO, cerver->queue_buffer_size);
    if (scheduler == NULL)
    {
        Log(FATAL, "[5XX]: Couldn't allocate memory to scheduler");
    }
    Log(TRACE, "Starting scheduler");

    ThreadPool *thread_pool = InitThreadPool(cerver->num_threads);
    if (thread_pool == NULL)
    {
        Log(FATAL, "[5XX]: Couldn't allocate memory to thread_pool\n");
    }
    StartThreads(cerver->router, scheduler, thread_pool);
    Log(TRACE, "Starting threads");
    Log(INFO, "vist http://localhost:%d", ntohs(cerver->address.sin_port));

    while (cerver_running)
    {
        client_socket = accept(cerver->server_fd, (struct sockaddr *)&cerver->address, (socklen_t *)&cerver->addrlen);
        if (errno == EINTR)
        {
            break;
        }
        else if (client_socket < 0)
        {
            // if (errno == EINTR)
            // {
            //     break;
            // }
            Log(ERROR, "[5XX]: Couldn't accect connections on socket\n");
        }
        else
        {
            // if (errno == EINTR)
            // {
            //     break;
            // }
            ScheduleRequestToBeHandled(scheduler, thread_pool, client_socket);
        }
    }
    close(client_socket);
    Log(ERROR, "Shutting Down...");
    FreeThreadPool(thread_pool);
    FreeScheduler(scheduler);
    FreeCerver(cerver);
    Log(ERROR, "bye bye...");
}

// WIP
void FreeCerver(Cerver *cerver)
{
    Log(ERROR, "FreeCerver");
    if (cerver != NULL)
    {
        if (cerver->router != NULL)
        {
            FreeRouter(cerver->router);
        }
        free(cerver);
    }
}
