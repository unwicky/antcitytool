#ifndef AT_HASHSET_H_
#define AT_HASHSET_H_

#include "util/at_hashmap.h"

typedef at_hashmap_t at_hashset_t;

#ifdef __cplusplus
extern "C" {
#endif

at_hashset_t *hashset_new(int, at_type_t);

void hashset_set_hash_func(at_hashset_t *, hash_func);

void hashset_set_equals_func(at_hashset_t *, equals_func);

void hashset_set_free_func(at_hashset_t *, free_func);

at_boolean_t hashset_insert(at_hashset_t *, void *);

at_boolean_t hashset_iscontain(at_hashset_t *, void *);

at_boolean_t hashset_remove(at_hashset_t *, void *);

const char *hashset_error();

int hashset_size(at_hashset_t *);

void hashset_free(at_hashset_t **);

#ifdef __cplusplus
}
#endif

#endif /* AT_HASHSET_H_ */
