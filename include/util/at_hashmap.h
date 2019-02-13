/**
* hashtable:key type can be long(int), double(float),char *(not NULL),
*           value type can be long(int), double(float), char * and any object type,
*           if value is an object, you can specify two functions:copyFunc-to copy an 
*           object value to hashtable;freeFunc-to release the memory of the object.
*           If no copyFunc specified, "=" will be used; if no freeFunc specified, we
*           just "free" the object
**/
#ifndef AT_HASHMAP_H_
#define AT_HASHMAP_H_

#include <string.h>
#include <stdlib.h>

#include "at_types.h"

#define DEFAULT_MAP_CAPCITY 32
#define DEFAULT_LOAD_FACTOR 0.75


#define LONGENTRY(k) ({\
            at_entry_t entry;\
            entry.type = LONGTYPE;\
            entry.long_entry = k;\
            entry;\
        })

#define DOUBLEENTRY(k) ({\
            at_entry_t entry;\
            entry.type = DOUBLETYPE;\
            entry.double_entry = k;\
            entry;\
        })

#define STRINGENTRY(k) ({\
            at_entry_t entry;\
            if (!k) {\
                entry.type = ILLEGALTYPE;\
            } else {\
                size_t len = strlen(k);\
                entry.type = STRINGTYPE;\
                entry.string_entry = (char *)malloc(len + 1);\
                strncpy(entry.string_entry, k, len + 1);\
            }\
            entry;\
        })

#define OBJECTENTRY(k, hash, equals, free) ({\
            at_entry_t entry;\
            if (!k) {\
                entry.type = ILLEGALTYPE;\
            } else {\
                entry.type = OBJECTTYPE;\
                entry.hash_func = hash;\
                entry.equals_func = equals;\
                entry.free_func = free;\
                entry.object_entry = k;\
            }\
            entry;\
        })

typedef enum {
    UNKNOWTYPE = 0,
    LONGTYPE = 1,
    DOUBLETYPE = 2,
    STRINGTYPE = 3,
    OBJECTTYPE = 4,
    ILLEGALTYPE = 99
} at_type_t;

typedef struct {
    union {
        long long_entry;
        double double_entry;
        char *string_entry;
        void *object_entry;
    };
    unsigned long (*hash_func)(void *);
    at_boolean_t (*equals_func)(void *, void *);
    void (*free_func)(void *);
    at_type_t type;
} at_entry_t;

typedef struct at_node_s at_node_t;
struct at_node_s {
    at_node_t *prev;
    at_entry_t key;
    at_entry_t value;
    at_node_t *next;
};

typedef struct {
    at_node_t *array;
    at_type_t key_type;
    at_type_t value_type;
    int capcity;
    float load_factor;
    int size;
    int threshold;
    int in_maintable;
    int in_list;
} at_hashmap_t;

typedef struct {
    at_node_t *next_node;
    at_entry_t *key, *value;
    int bucket_index;
} at_map_iterator_t;

#ifdef __cplusplus
extern "C" {
#endif

at_hashmap_t *hashmap_new(int);

void hashmap_setloadfactor(at_hashmap_t *, double);

at_boolean_t hashmap_insert(at_hashmap_t *, at_entry_t, at_entry_t);

at_map_iterator_t hashmap_iterate(at_hashmap_t *);

at_map_iterator_t *hashmap_next(at_hashmap_t *, at_map_iterator_t *);

at_entry_t *hashmap_get(at_hashmap_t *, at_entry_t);

at_boolean_t hashmap_remove(at_hashmap_t *, at_entry_t);

const char *hashmap_error();

int hashmap_size(at_hashmap_t *);

void hashmap_dump(at_hashmap_t *);

void hashmap_free(at_hashmap_t **);

#ifdef __cplusplus
}
#endif

#endif
