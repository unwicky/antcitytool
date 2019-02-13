#ifndef AT_STACK_H_
#define AT_STACK_H_

#define DEFAULT_STACK_SIZE 16

#include "at_types.h"

typedef struct {
    void **array;
    int cursor;
    int capcity, size;
} at_stack_t;

#ifdef __cplusplus
extern "C" {
#endif

at_stack_t *stack_new(int cap);

void stack_push(at_stack_t *stack, void *node);

void *stack_pop(at_stack_t *stack);

at_boolean_t stack_isempty(at_stack_t *stack);

int stack_size(at_stack_t *stack);

void stack_free(at_stack_t **stack_ptr);

#ifdef __cplusplus
}
#endif

#endif /* AT_STACK_H_ */
