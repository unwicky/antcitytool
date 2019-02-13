#include "util/at_hashset.h"

at_hashset_t *hashset_new(int capcity) {
    return hashmap_new(capcity);
}

at_boolean_t hashset_insert(at_hashset_t *set, at_entry_t entry) {
    return hashmap_insert(set, entry, LONGENTRY(0));
}

at_boolean_t hashset_iscontain(at_hashset_t *set, at_entry_t entry) {
    at_entry_t *ret = hashmap_get(set, entry);
    if (ret) {
        return AT_TRUE;
    } else {
        return AT_FALSE;
    }
}

at_boolean_t hashset_remove(at_hashset_t *set, at_entry_t entry) {
    return hashmap_remove(set, entry);
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

