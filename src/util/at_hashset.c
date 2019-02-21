#include "util/at_hashset.h"

at_hashset_t *hashset_new(int capcity, at_type_t type) {
    return hashmap_new(capcity, type, INTTYPE);
}

void hashset_set_hash_func(at_hashset_t *set, hash_func f) {
    hashmap_set_key_hash_func(set, f); 
}

void hashset_set_equals_func(at_hashset_t *set, equals_func f) {
    hashmap_set_key_equals_func(set, f);
}

void hashset_set_free_func(at_hashset_t *set, free_func f) {
    hashmap_set_key_free_func(set, f);
}

at_boolean_t hashset_insert(at_hashset_t *set, void *data) {
    int placeholder = 0;
    return hashmap_insert(set, data, &placeholder);
}

at_boolean_t hashset_iscontain(at_hashset_t *set, void *data) {
    at_entry_t *ret = hashmap_get(set, data);
    if (ret) {
        return AT_TRUE;
    } else {
        return AT_FALSE;
    }
}

at_boolean_t hashset_remove(at_hashset_t *set, void *data) {
    return hashmap_remove(set, data);
}

const char *hashset_error() {
    return hashmap_error();
}

int hashset_size(at_hashset_t *set) {
    return hashmap_size(set);
}

void hashset_free(at_hashset_t **set) {
    hashmap_free(set);
}

