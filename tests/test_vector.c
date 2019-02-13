#include <stdio.h>

#include "util/at_vector.h"

typedef struct {
    int pos;
    double score;
    char name[12];
} Node;

int main(int argc, char *argv[]) {
    at_vector_t *vector = vector_new(10, sizeof(Node), NULL);
    Node node = {0};
    int i = 0;
    for (i = 0; i < 10; i++) {
        node.pos = 1;
        node.score = i / (double)3;
        memset(node.name, 0, 12);
        snprintf(node.name, 12, "root%d", i);
        vector_insert(vector, &node);
    }
    Node *temp = vector_get(vector, 0);
    printf("pos: %d, score: %lf, name: %s\n", temp->pos, temp->score, temp->name);
    temp = vector_get(vector, 5);
    printf("pos: %d, score: %lf, name: %s\n", temp->pos, temp->score, temp->name);
    printf("to iterate vector:\n");
    at_vector_iterator_t primer_itr = vector_iterate(vector);
    while ((temp = vector_next(vector, &primer_itr)) != NULL) {
        printf("pos: %d, score: %lf, name: %s\n", temp->pos, temp->score, temp->name);
    }
    vector_free(&vector);
    return 0;
}
