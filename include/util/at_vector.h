#ifndef AT_VECTOR_H_
#define AT_VECTOR_H_

#include <string.h>

typedef void (*ele_free_func)(void *);

typedef struct {
    void *array;
    int capcity;
    int size;
    int element_size;
    ele_free_func free_func;
} at_vector_t;

typedef struct {
    void *data;
    int index;
} at_vector_iterator_t;

#ifdef __cplusplus
extern "C" {
#endif

at_vector_t *vector_new(int, int, ele_free_func);

int vector_size(at_vector_t *);

void *vector_get(at_vector_t *, int);

void *vector_prefetch(at_vector_t *);

void vector_insert(at_vector_t *, void *);

at_vector_iterator_t vector_iterate(at_vector_t *);

void *vector_next(at_vector_t *, at_vector_iterator_t *);

void vector_reset(at_vector_t *);

void vector_free(at_vector_t **);

#ifdef __cplusplus
}
#endif

#endif /* AT_VECTOR_H_ */
