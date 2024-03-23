#include <pthread.h>

#include "./scheduler.h"
#include "./queue.h"
#include "./worker.h"

#include "./../http/request/request.h"

void *ThreadWorker(void *arg)
{
    ThreadArg *args = (ThreadArg *)arg;
    while (1)
    {
        int *client_socket = GetFromScheduler(args->scheduler, args->workers);
        HandleRequest(client_socket);
    }
    return NULL;
}