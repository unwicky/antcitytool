#include <stdio.h>

#include "util/at_queue.h"

int main(int argc, char *argv[]) {
    at_queue_t *queue = queue_new(sizeof(int));
    int i = 0;
    for (i = 0; i < 16; i++) {
        queue_pushback(queue, &i);
    }
    queue_pop(queue);
    queue_pop(queue);
    for (i = 16; i < 20; i++) {
        queue_pushback(queue, &i);
    }
    printf("queueSize: %d\n", queue_size(queue));
    while (!queue_isempty(queue)) {
        int *temp = (int *)queue_pop(queue);
        printf("%d\n", *temp);
    }
    queue_free(&queue);
    return 0;
}
