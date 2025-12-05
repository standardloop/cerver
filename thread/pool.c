#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <standardloop/logger.h>

#include "./scheduler.h"
#include "./../structures/queue/queue.h"
#include "./worker.h"

ThreadPool *InitThreadPool(int num_threads)
{
    ThreadPool *workers = (ThreadPool *)malloc(sizeof(ThreadPool));
    if (workers == NULL)
    {
        return NULL;
    }
    workers->num_threads = num_threads;
    workers->working_threads = 0;
    workers->pool = (pthread_t *)malloc(num_threads * sizeof(pthread_t));

    int mutex_init = pthread_mutex_init(&workers->LOCK, NULL);
    if (mutex_init != 0)
    {
        Log(FATAL, "pthread_mutex_init failed!");
    }
    int fill_init = pthread_cond_init(&workers->FILL, NULL);
    if (fill_init != 0)
    {
        Log(FATAL, "fill_init pthread_cond_init failed!");
    }
    int empty_init = pthread_cond_init(&workers->EMPTY, NULL);
    if (empty_init != 0)
    {
        Log(FATAL, "empty_init pthread_cond_init failed!");
    }

    return workers;
}

void StartThreads(Router *router, Scheduler *scheduler, ThreadPool *workers)
{
    for (int i = 0; i < workers->num_threads; i++)
    {
        ThreadArg *arg = (ThreadArg *)malloc(sizeof(ThreadArg));
        if (arg == NULL)
        {
            Log(ERROR, "[ERROR][5XX]: Thread number %d: Creation failed", i);
        }
        else
        {
            arg->workers = workers;
            arg->scheduler = scheduler;
            arg->worker_number = i;
            arg->router = router;
            int check_thread = pthread_create(&workers->pool[i], NULL, ThreadWorker, arg);
            if (check_thread != 0)
            {
                Log(ERROR, "[ERROR][5XX]: Thread number %d: Creation failed", i);
            }
        }
    }
}

void ShutdownThreadPool(ThreadPool *thread_pool)
{
    if (thread_pool != NULL)
    {
        if (thread_pool->pool != NULL && thread_pool->num_threads > 0)
        {
            for (int i = 0; i < thread_pool->num_threads; i++)
            {
                pthread_join(thread_pool->pool[i], NULL);
            }
        }
    }
    pthread_mutex_destroy(&thread_pool->LOCK);
    pthread_cond_destroy(&thread_pool->EMPTY);
    pthread_cond_destroy(&thread_pool->FILL);
    pthread_cond_destroy(&thread_pool->SIG);
}

// FIXME mutex?
void FreeThreadPool(ThreadPool *thread_pool)
{
    Log(ERROR, "FreeThreadPool");
    if (thread_pool->pool != NULL)
    {
        free(thread_pool->pool);
    }
    free(thread_pool);
}
