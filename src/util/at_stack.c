#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "util/at_stack.h"

static void expand_stack(at_stack_t *stack) {
    int new_cap = stack->capcity * 3 / 2 + 1, i = 0;
    stack->array = realloc(stack->array, new_cap * sizeof(void *));
    stack->capcity = new_cap;
}

at_stack_t *stack_new(int cap) {
    at_stack_t *stack = NULL;
    void **buffer = (void **)malloc(cap * sizeof(void *));
    if (buffer) {
        stack = (at_stack_t *)malloc(sizeof(at_stack_t));
        if (stack) {
            stack->array = buffer;
            stack->cursor = -1;
            stack->size = 0;
            stack->capcity = cap;
        } else {
            free(buffer);
        }
    }
    return stack;
}

void stack_push(at_stack_t *stack, void *node) {
    if (stack->size >= stack->capcity) {
        expand_stack(stack);
    }
    stack->array[++stack->cursor] = node;
    stack->size++;
}

void *stack_pop(at_stack_t *stack) {
    void *target = NULL;
    if (stack->size > 0) {
        target = stack->array[stack->cursor--];
        stack->size--;
    }
    return target;
}

at_boolean_t stack_isempty(at_stack_t *stack) {
    return stack->size == 0;
}

int stack_size(at_stack_t *stack) {
    return stack->size;
}

void stack_free(at_stack_t **stack_ptr) {
    at_stack_t *stack = *stack_ptr;
    if (stack) {
        free(stack->array);
        free(stack);
        *stack_ptr = NULL;
    }
}
