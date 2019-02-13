#include <stdio.h>
#include <stdlib.h>

#include "util/at_stack.h"

int main(int argc, char *argv[]) {
    at_stack_t *stack = stack_new(16);
    int i = 0;
    int *j = NULL;
    for (i = 0; i < 18; i++) {
        j = malloc(sizeof(int));
        *j = i;
        if (i == 16) {
            stack_pop(stack);
        }
        stack_push(stack, j);
    }
    while (!stack_isempty(stack)) {
        int *temp = stack_pop(stack);
        printf("%d\n", *temp);
        free(temp);
    }
    stack_free(&stack);
    return 0;
}
