#ifndef AT_HASHMAP_H_
#define AT_HASHMAP_H_

#include <string.h>
#include <stdlib.h>

#include "at_types.h"

#define DEFAULT_MAP_CAPCITY 32
#define DEFAULT_LOAD_FACTOR 0.75

typedef unsigned long (*hash_func)(void *);
typedef at_boolean_t (*equals_func)(void *, void *);
//typedef void (*free_func)(void *);

typedef enum {
    INTTYPE = 1,
    LONGTYPE = 2,
    FLOATTYPE = 3,
    DOUBLETYPE = 4,
    STRINGTYPE = 5,
    OBJECTTYPE = 6
} at_type_t;

typedef union {
    int int_entry;
    long long_entry;
    float float_entry;
    double double_entry;
    char *string_entry;
    void *object_entry;
} at_entry_t;

typedef struct at_node_s at_node_t;
struct at_node_s {
    at_node_t *prev;
    at_entry_t key;
    at_entry_t value;
    at_node_t *next;
    at_boolean_t is_inuse;
};

typedef struct {
    at_node_t *buckets;
    at_type_t key_type;
    unsigned long (*key_hash_fn)(void *);
    at_boolean_t (*key_equals_fn)(void *, void *);
    void (*key_free_fn)(void *);
    at_type_t val_type;
    void (*val_free_fn)(void *);
    int capcity;
    float load_factor;
    int size;
    int threshold;
} at_hashmap_t;

typedef struct {
    at_node_t *next_node;
    void *key, *value;
    int bucket_index;
} at_map_iterator_t;

#ifdef __cplusplus
extern "C" {
#endif

at_hashmap_t *hashmap_new(int, at_type_t, at_type_t);

void hashmap_set_key_hash_func(at_hashmap_t *, hash_func);

void hashmap_set_key_equals_func(at_hashmap_t *, equals_func);

/*void hashmap_set_key_free_func(at_hashmap_t *, free_func);

void hashmap_set_val_free_func(at_hashmap_t *, free_func);*/

void hashmap_setloadfactor(at_hashmap_t *, double);

at_boolean_t hashmap_insert(at_hashmap_t *, void *, void *);

at_map_iterator_t hashmap_iterate(at_hashmap_t *);

at_map_iterator_t *hashmap_next(at_hashmap_t *, at_map_iterator_t *);

void *hashmap_get(at_hashmap_t *, void *);

at_boolean_t hashmap_remove(at_hashmap_t *, void *);

const char *hashmap_error();

int hashmap_size(at_hashmap_t *);

void hashmap_dump(at_hashmap_t *);

void hashmap_free(at_hashmap_t **);

#ifdef __cplusplus
}
#endif

#endif
