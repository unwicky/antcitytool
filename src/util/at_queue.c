#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "util/at_queue.h"

static void expand_queue(at_queue_t *queue) {
    int new_cap = queue->capcity * 3 / 2 + 1;
    void *new_array = malloc(new_cap * queue->node_size);
    memcpy(new_array, queue->array + queue->head * queue->node_size, 
        (queue->capcity - queue->head) * queue->node_size);
    memcpy(new_array + (queue->capcity - queue->head) * queue->node_size, 
        queue->array, queue->head * queue->node_size);
    free(queue->array);
    queue->array = new_array;
    queue->head = 0;
    queue->tail = queue->capcity;
    queue->capcity = new_cap;
}

at_queue_t *queue_new(int node_size) {
    assert(node_size > 0);
    at_queue_t *queue = NULL;
    void *buffer = malloc(DEFAULT_QUEUE_SIZE * node_size);
    if (buffer) {
        queue = (at_queue_t *)malloc(sizeof(at_queue_t));
        if (!queue) {
            free(buffer);
        } else {
            queue->array = buffer;
            queue->head = 0;
            queue->tail = 0;
            queue->capcity = DEFAULT_QUEUE_SIZE;
            queue->size = 0;
            queue->node_size = node_size;
        }
    }
    return queue;
}

void queue_pushback(at_queue_t *queue, void *node) {
    if (queue->size >= queue->capcity) {
        expand_queue(queue);
    }
    memcpy(queue->array + queue->tail * queue->node_size, node, queue->node_size);
    queue->size++;
    queue->tail++;
    if (queue->tail >= queue->capcity) {
        queue->tail -= queue->capcity;
    }
}

void *queue_pop(at_queue_t *queue) {
    void *target = NULL;
    if (queue->size > 0) {
        target = queue->array + queue->head * queue->node_size;
        queue->head++;
        if (queue->head >= queue->capcity) {
            queue->head -= queue->capcity;
        }
        queue->size--;
    }
    return target;
}

at_boolean_t queue_isempty(at_queue_t *queue) {
    return queue->size == 0;
}

int queue_size(at_queue_t *queue) {
    return queue->size;
}

void queue_free(at_queue_t **queue_ptr) {  
    at_queue_t *queue = *queue_ptr;
    if (queue) {
        free(queue->array);
        free(queue);
        *queue_ptr = NULL;
    }
}
