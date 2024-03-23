#include "./pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
ThreadPool *InitThreadPool(int num_threads)
{

    ThreadPool *workers = (ThreadPool *)malloc(sizeof(ThreadPool));
    if (workers == NULL)
    {
        return NULL;
    }
    workers->num_threads = num_threads;
    workers->working_threads = 0;
    workers->pool = malloc(num_threads * sizeof(pthread_t));

    Pthread_mutex_init(&workers->LOCK, NULL);
    Pthread_cond_init(&workers->FILL, NULL);
    Pthread_cond_init(&workers->EMPTY, NULL);

    return workers;
}
