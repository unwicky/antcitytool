/*
 * test_thread_pool.c
 *
 *  Created on: 2016年4月6日
 *      Author: rengaohe
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "util/at_thread_pool.h"

static at_thread_pool_t *pool;

static void free_task(at_task_t **task_ptr) {
    at_task_t *task = *task_ptr;
    if (task) {
        free(task->params);
        free(task);
        *task_ptr = NULL;
    }
}

static void print(int *i) {
    printf("%ld:i=%d\n", pthread_self(), *i);
    if (*i > 10) {
        return ;
    }
    int *p = NULL;
    int j = 0, start = (*i) * 10;
    for (j = 0; j < 10; j++) {
        at_task_t *task = (at_task_t *)malloc(sizeof(at_task_t));
        task->run = (void *)print;
        p = malloc(sizeof(int));
        *p = start + j;
        task->params = p;
        task->free_task = free_task;
        thread_pool_add_task(pool, task);
    }
    usleep(600000);
}

int main(int argc, char *argv[]) {
    int i = 0;
    at_task_t *task = NULL;
    pool = thread_pool_create(8);
    int *p = NULL;
    for (i = 1; i < 10; i++) {
        task = (at_task_t *)malloc(sizeof(at_task_t));
        task->run = (void *)print;
        p = malloc(sizeof(int));
        *p = i;
        task->params = p;
        task->free_task = free_task;
        thread_pool_add_task(pool, task);
    }
    while (!thread_pool_is_all_idle(pool)) {
        usleep(10000);
    }
    thread_pool_stop(&pool);
    return 0;
}

