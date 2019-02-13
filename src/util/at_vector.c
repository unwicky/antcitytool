#include <stdlib.h>

#include "util/at_vector.h"

static void vector_realloc(at_vector_t *vector) {
    int new_cap = vector->capcity * 3 / 2 + 1;
    vector->array = realloc(vector->array, new_cap * vector->element_size);
    vector->capcity = new_cap;
}

at_vector_t *vector_new(int capcity, int element_size, ele_free_func func) {
    at_vector_t *vector = (at_vector_t *)malloc(sizeof(at_vector_t));
    vector->capcity = capcity;
    vector->size = 0;
    vector->element_size = element_size;
    vector->array = malloc(capcity * element_size);
    vector->free_func = func;
    return vector;
}

int vector_size(at_vector_t *vector) {
    return vector->size;
}

void *vector_get(at_vector_t *vector, int index) {
    if (index >= vector->size || index < 0) {
        return NULL;
    }
    return vector->array + index * vector->element_size;
}

/**
* get an unused memory from vector <vector>, the memory will be taken
* as a new element, whether or not it is used by the applicant
**/
void *vector_prefetch(at_vector_t *vector) {
    if (vector->size >= vector->capcity) {
        vector_realloc(vector);
    }
    return vector->array + vector->size++ * vector->element_size;
}

void vector_insert(at_vector_t *vector, void *ele) {
    if (vector->size >= vector->capcity) {
        vector_realloc(vector);
    }
    memcpy(vector->array + vector->size++ * vector->element_size , ele, vector->element_size);
}

at_vector_iterator_t vector_iterate(at_vector_t *vector) {
    at_vector_iterator_t itr = {0};
    itr.data = vector->array;
    itr.index = 0;
    return itr;
}

void *vector_next(at_vector_t *vector, at_vector_iterator_t *primer_itr) {
    void *ret = NULL;
    if (primer_itr->index++ < vector->size) {
        ret = primer_itr->data;
        primer_itr->data += vector->element_size;
    }
    return ret;
}

void vector_reset(at_vector_t *vector) {
    vector->size = 0;
}

void vector_free(at_vector_t **vector_ptr) {
    at_vector_t *vector = *vector_ptr;
    int i = 0;
    if (vector->free_func) {
        for (i = 0; i < vector->size; i++) {
            vector->free_func(vector->array + i * vector->element_size);
        }
    }
    free(vector->array);
    vector->size = 0;
    vector->capcity = 0;
    free(vector);
    *vector_ptr = NULL;
}

