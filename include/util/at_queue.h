#ifndef AT_QUEUE_H_
#define AT_QUEUE_H_

#include "at_types.h"

#define DEFAULT_QUEUE_SIZE 16

typedef struct {
    void *array;
    int head, tail;
    int capcity, size, node_size;
} at_queue_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
* create an empty queue,the size of every node in the queue 
* is <nodeSize>
**/
at_queue_t *queue_new(int node_size);
/**
* push an node <node> to the tail of the queue, the node will be
* copied to the queue, so if the node is alloced in you program, you
* must free it anywhere by yourself, and be careful, if another pointer
* is contained in the node, the pointer is kept in the queue, and it must
* be freed anywhere by yourself too
**/
void queue_pushback(at_queue_t *queue, void *node);
/**
* pop out an node from the head of the queue, you need not to free the node,
* but be careful, if another pointer is contained in the node,you must free
* it anywhere by your self
**/
void *queue_pop(at_queue_t *queue);

at_boolean_t queue_isempty(at_queue_t *queue);

int queue_size(at_queue_t *queue);

void queue_free(at_queue_t **queue_ptr);

#ifdef __cplusplus
}
#endif

#endif /*AT_QUEUE_H_*/
