#include <stdio.h>
#include <stdlib.h>

#include "util/at_thread_pool.h"

static void run(at_thread_pool_t *pool) {
    at_task_t *task = NULL;
    while (pool->status != STOPPED) {
        pthread_mutex_lock(&pool->lock);
        if ((task = stack_pop(pool->tasks)) != NULL) {
            pthread_mutex_unlock(&pool->lock);
            task->run(task->params);
            task->free_task(&task);
        } else {
            if (pool->status == RUNNING) {
                pool->idles++;
                pthread_cond_wait(&pool->cond, &pool->lock);
                pool->idles--;
                pthread_mutex_unlock(&pool->lock);
            } else {
                pthread_mutex_unlock(&pool->lock);
                break;
            }
        }
    }
}

at_thread_pool_t *thread_pool_create(int size) {
    int i = 0;
    at_thread_pool_t *pool = (at_thread_pool_t *)malloc(sizeof(at_thread_pool_t));
    pool->size = size;
    pool->idles = 0;
    pool->threads = (pthread_t *)malloc(size * sizeof(pthread_t));
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->cond, NULL);
    pool->tasks = stack_new(2 * size);
    pool->status = RUNNING;
    for (i = 0; i < size; i++) {
        pthread_create(&pool->threads[i], NULL, (void *)run, pool);
    }
    return pool;
}

at_boolean_t thread_pool_add_task(at_thread_pool_t *pool, at_task_t *task) {
    if (pool->status != RUNNING) {
        return AT_FALSE;
    }
    pthread_mutex_lock(&pool->lock);
    stack_push(pool->tasks, task);
    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->lock);
    return AT_TRUE;
}

at_boolean_t thread_pool_is_all_idle(at_thread_pool_t *pool) {
    pthread_mutex_lock(&pool->lock);
    at_boolean_t is_all_idle = pool->idles == pool->size
        && stack_isempty(pool->tasks);
    pthread_mutex_unlock(&pool->lock);
    return is_all_idle;
}

at_boolean_t thread_pool_stop(at_thread_pool_t **pool_ptr) {
    at_thread_pool_t *pool = *pool_ptr;
    int i = 0;
    if (pool) {
        pool->status = STOPPING;
        pthread_mutex_lock(&pool->lock);
        pthread_cond_broadcast(&pool->cond);
        pthread_mutex_unlock(&pool->lock);
        for(i = 0; i < pool->size; i++) {
            pthread_join(pool->threads[i], NULL);
        }
        free(pool->threads);
        pthread_cond_destroy(&pool->cond);
        pthread_mutex_destroy(&pool->lock);
        stack_free(&pool->tasks);
        pool->status = STOPPED;
        free(pool);
        *pool_ptr = NULL;
    }
    return AT_TRUE;
}
