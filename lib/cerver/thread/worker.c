#include <pthread.h>

#include "./scheduler.h"
#include "./queue.h"
#include "./pool.h"
#include "./worker.h"

#include "./../http/request/request.h"

void *ThreadWorker(void *arg)
{
    ThreadArg *args = (ThreadArg *)arg;
    while (1)
    {
        int *client_socket = GetFromScheduler(args->workers, args->scheduler);
        HandleRequest(client_socket);
    }
    return NULL;
}