#include <pthread.h>
#include <signal.h>
#include <errno.h>

#include "./scheduler.h"
#include "./../structures/queue/queue.h"
#include "./worker.h"

#include "./../http/handler.h"
#include <standardloop/util.h>
#include <standardloop/logger.h>

volatile sig_atomic_t global_worker_running = true;

static void threadWorkerSigHandler(int);

static void threadWorkerSigHandler(int signum)
{
    Log(ERROR, "Worker thread: Received SIGUSR1 signal (%d).\n", signum);
    global_worker_running = false;
    // pthread_exit(NULL);
}

void *ThreadWorker(void *arg)
{
    struct sigaction sa;
    sa.sa_handler = threadWorkerSigHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        Log(ERROR, "Worker thread: sigaction failed");
        pthread_exit(NULL);
    }

    ThreadArg *args = (ThreadArg *)arg;
    while (global_worker_running)
    {
        if (errno == EINTR)
        {
            break;
        }
        int client_socket = AcceptRequest(args->scheduler, args->workers);
        if (client_socket == -1)
        {
            break;
        }
        HandleRequest(args->router, client_socket);
    }
    pthread_mutex_unlock(&args->workers->LOCK);
    Log(ERROR, "finishing up requests in queue...");
    while (args->scheduler->curr_size > 0)
    {
        int client_socket = AcceptRequest(args->scheduler, args->workers);
        HandleRequest(args->router, client_socket);
    }
    Log(ERROR, "done");
    // pthread_exit(NULL);
    return NULL;
}
