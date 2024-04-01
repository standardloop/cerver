#include <pthread.h>

#include "./scheduler.h"
#include "./queue/queue.h"
#include "./worker.h"

#include "./../http/handler.h"
#include "./../util/util.h"

void *ThreadWorker(void *arg)
{
    ThreadArg *args = (ThreadArg *)arg;
    while (ALWAYS)
    {
        int client_socket = AcceptRequest(args->scheduler, args->workers);
        HandleRequest(args->router, client_socket);
    }
    return NULL;
}
