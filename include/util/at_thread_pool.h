#ifndef AT_THREAD_POOL_H__
#define AT_THREAD_POOL_H__

#include <pthread.h>

#include "at_types.h"
#include "util/at_stack.h"

typedef enum {
    RUNNING,
    STOPPING,
    STOPPED
} at_thread_pool_status_t;

typedef struct at_task_s at_task_t;

struct at_task_s {
    void *(*run)(void *);
    void *params;
    void (*free_task)(at_task_t **);
};

typedef struct {
    int size;
    int idles;
    at_thread_pool_status_t status;
    pthread_t *threads;
    pthread_cond_t cond;
    pthread_mutex_t lock;
    at_stack_t *tasks;
} at_thread_pool_t;

#ifdef __cplusplus
extern "C" {
#endif

at_thread_pool_t *thread_pool_create(int);

at_boolean_t thread_pool_add_task(at_thread_pool_t *, at_task_t *);

at_boolean_t thread_pool_is_all_idle(at_thread_pool_t *);

at_boolean_t thread_pool_stop(at_thread_pool_t **);

#ifdef __cplusplus
}
#endif

#endif /*end AT_THREAD_POOL_H__*/
