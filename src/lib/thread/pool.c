#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "./scheduler.h"
#include "./queue/queue.h"
#include "./worker.h"

ThreadPool *InitThreadPool(int num_threads)
{
    ThreadPool *workers = (ThreadPool *)malloc(sizeof(ThreadPool));
    if (workers == NULL)
    {
        printf("\n");
        return NULL;
    }
    workers->num_threads = num_threads;
    workers->working_threads = 0;
    workers->pool = malloc(num_threads * sizeof(pthread_t));

    int mutex_init = pthread_mutex_init(&workers->LOCK, NULL);
    if (mutex_init != 0)
    {
        printf("\n mutex_init \n");
    }
    int fill_init = pthread_cond_init(&workers->FILL, NULL);
    if (fill_init != 0)
    {
        printf("\n fill_init \n");
    }
    int empty_init = pthread_cond_init(&workers->EMPTY, NULL);
    if (empty_init != 0)
    {
        printf("\n empty_init \n");
    }

    return workers;
}

void StartThreads(Scheduler *scheduler, ThreadPool *workers)
{
    for (int i = 0; i < workers->num_threads; i++)
    {
        ThreadArg *arg = (ThreadArg *)malloc(sizeof(ThreadArg));
        if (arg == NULL)
        {
            printf("\n[ERROR][5XX]: Thread number %d: Creation failed", i);
            continue;
        }
        else
        {
            arg->workers = workers;
            arg->scheduler = scheduler;
            arg->worker_number = i;
            int check_thread = pthread_create(&workers->pool[i], NULL, ThreadWorker, arg);
            if (check_thread != 0)
            {
                printf("\n[ERROR][5XX]: Thread number %d: Creation failed", i);
            }
        }
    }
}

// FIXME mutex?
void FreeThreadPool(ThreadPool *thread_pool)
{
    if (thread_pool->pool != NULL)
    {
        free(thread_pool->pool);
    }
    free(thread_pool);
}
