#include <stdio.h>

#include "util/at_hashmap.h"
#include "util/at_util.h"

static char error_msg[128];

at_boolean_t is_map_integrity(at_hashmap_t *map) {
    if (map->key_type == OBJECTTYPE && (!map->key_hash_fn || !map->key_equals_fn)) {
         return AT_FALSE;
    }
    return AT_TRUE;
}

static unsigned long key_hashcode(at_hashmap_t *map, void *key, unsigned int mask) {
    unsigned long hash = 1;
    float f_key;
    int i_key;
    switch (map->key_type) {
    case INTTYPE:
        i_key = *(int *)key;
        hash = (unsigned long)i_key;
        break;
    case LONGTYPE:
        hash = *((unsigned long *)key);
        break;
    case FLOATTYPE:
        f_key = *(float *)key;
        hash = double_hashcode((double)f_key);
        break;
    case DOUBLETYPE:
	hash = double_hashcode(*((double *)key));
        break;
    case STRINGTYPE:
        hash = simple_hash((char *)key);
        break;
    case OBJECTTYPE:
        hash = map->key_hash_fn(key);
        break;
    default:
        hash = 1;
        break;
    }
    return hash & (mask - 1);
}

static at_boolean_t is_equals(at_hashmap_t *map, at_entry_t *entry, void* key) {
    switch (map->key_type) {
    case INTTYPE:
        return entry->int_entry == *(int *)key;
    case LONGTYPE:
        return entry->long_entry == *(long *)key;
    case FLOATTYPE:
        return entry->float_entry == *(float *)key;
    case DOUBLETYPE:
        return entry->double_entry == *(double *)key;
    case STRINGTYPE:
        return strcmp(entry->string_entry, (char *)key) == 0;
    case OBJECTTYPE:
        return map->key_equals_fn(entry->object_entry, key);
    default:
        return AT_FALSE;
    }
}

static void set_entry(at_entry_t *entry, at_type_t type, void *v) {
    char *str_v;
    int len = 0;
    switch(type) {
    case INTTYPE:
        entry->int_entry = *(int *)v;
        break;
    case LONGTYPE:
        entry->long_entry = *(long *)v;
        break;
    case FLOATTYPE:
        entry->float_entry = *(float *)v;
        break;
    case DOUBLETYPE:
        entry->double_entry = *(double *)v;
        break;
    case STRINGTYPE:
        /*str_v = (char *)v;
        len = strlen(str_v);
        entry->string_entry = (char *)malloc(len + 1);
        strncpy(entry->string_entry, str_v, len + 1);*/
        entry->string_entry = (char *)v;
        break;
    case OBJECTTYPE:
        entry->object_entry = v;
        break;
    }
}

void *get_entry(at_entry_t *entry, at_type_t type) {
    switch(type) {
    case INTTYPE:
        return &entry->int_entry;
    case LONGTYPE:
        return &entry->long_entry;
    case FLOATTYPE:
        return &entry->float_entry;
    case DOUBLETYPE:
        return &entry->double_entry;
    case STRINGTYPE:
        return entry->string_entry;
    case OBJECTTYPE:
        return entry->object_entry;
    }
}

/*static void free_entry(at_type_t type, at_entry_t *entry, free_func fn) {
    if (type == STRINGTYPE) {
        free(entry->string_entry);
    } else if (type == OBJECTTYPE) {
        if (fn) {
            fn(entry->object_entry);
        } else {
            free(entry->object_entry);
        }
    }
}

static void free_node(at_hashmap_t *map, at_node_t *node) {
    free_entry(map->key_type, &node->key, map->key_free_fn);
    free_entry(map->val_type, &node->value, map->val_free_fn);
}*/

static at_boolean_t rehash(at_hashmap_t *map, at_node_t *node, at_node_t *new_buckets, int hash_mask) {
    void *raw_key = get_entry(&node->key, map->key_type);
    unsigned int newIndex =  key_hashcode(map, raw_key, hash_mask);
    at_node_t *ptr = &new_buckets[newIndex];
    if (!ptr->is_inuse) {
	ptr->key = node->key;
        ptr->value = node->value;
	ptr->is_inuse = node->is_inuse;
    } else {
        at_node_t *new_node = (at_node_t *)malloc(sizeof(at_node_t));
        if (!new_node) {
	    return AT_FALSE;
	}
        new_node->key = node->key;
        new_node->value = node->value;
	new_node->is_inuse = node->is_inuse;
	new_node->prev = ptr;
        new_node->next = ptr->next;
        if (ptr->next) {
	    ptr->next->prev = new_node;
	}
	ptr->next = new_node;
    }
    return AT_TRUE;
}

static at_boolean_t expand_map(at_hashmap_t *map) {
    at_boolean_t ret = AT_TRUE;
    int new_capcity = map->capcity << 1, count = 0, i = 0;
    at_node_t *new_buckets = (at_node_t *)calloc(new_capcity, sizeof(at_node_t));
    if (!new_buckets) {
        strncpy(error_msg, "expand map failed:out of memory", 51);
	return AT_FALSE;
    }
    for (i = 0; count < map->size && i < map->capcity; i++) {
        at_node_t *ptr1 = &map->buckets[i], *ptr2 = NULL;
        if(ptr1->is_inuse) {
	    at_boolean_t is_in_list = AT_FALSE;
	    while (ptr1) {
	        ret = rehash(map, ptr1, new_buckets, new_capcity);
                if (!ret) {
		    strncpy(error_msg, "expand map failed:out of memory,map has been broken", 51);
		    return AT_FALSE;
                }
		count++;
		ptr2 = ptr1->next;
		if (is_in_list) {
		    free(ptr1);
		} else {
		    is_in_list = AT_TRUE;
		}
                ptr1 = ptr2;
	    }               
        }
    }
    free(map->buckets);
    map->buckets = new_buckets;
    map->capcity = new_capcity;
    map->threshold = new_capcity * map->load_factor;
    return AT_TRUE;
}

static at_node_t *next_valid_bucket(at_hashmap_t *map, int *bucket_index) {
    at_node_t *node_itr = NULL;
    int i = *bucket_index;
    for (; i < map->capcity; i++) {
        if (map->buckets[i].is_inuse) {
            node_itr = &map->buckets[i];
            *bucket_index = i;
            break;
        }
    }
    return node_itr;
}

void hashmap_dump(at_hashmap_t *map) {
    int count = 0, i = 0; 
    for (i = 0; i < map->capcity; i++) {
        at_node_t *ptr = &map->buckets[i];
        if (ptr->is_inuse) {
            while (ptr) {
                printf("key: %s, value: %s\n", ptr->key.string_entry, ptr->value.string_entry);
                ptr = ptr->next;
            }
        }
    }
}

at_hashmap_t *hashmap_new(int capcity, at_type_t key_type, at_type_t val_type) {
    if (capcity <= 0) {
        capcity = DEFAULT_MAP_CAPCITY;
    }
    at_hashmap_t *map = (at_hashmap_t *)malloc(sizeof(at_hashmap_t));
    map->capcity = min2n(capcity);
    map->buckets = (at_node_t *)calloc(map->capcity, sizeof(at_node_t));
    map->key_type = key_type;
    map->val_type = val_type;
    map->load_factor = DEFAULT_LOAD_FACTOR;
    map->size = 0;
    map->threshold = map->load_factor * map->capcity;
    return map;
}

void hashmap_set_key_hash_func(at_hashmap_t *map, hash_func fn) {
    map->key_hash_fn = fn;
}

void hashmap_set_key_equals_func(at_hashmap_t *map, equals_func fn) {
    map->key_equals_fn = fn;
}

/*void hashmap_set_key_free_func(at_hashmap_t *map, free_func fn) {
    map->key_free_fn = fn;
}

void hashmap_set_val_free_func(at_hashmap_t *map, free_func fn) {
    map->val_free_fn = fn;
}*/

void hashmap_setloadfactor(at_hashmap_t *map, double load_factor) {
    if (load_factor <= 0) {
        map->load_factor = load_factor;
        map->threshold = map->load_factor * map->capcity;
    }
}

at_boolean_t hashmap_insert(at_hashmap_t *map, void *key, void *value) {
    memset(error_msg, 0, 64);
    if (!is_map_integrity(map)) {
        strncpy(error_msg, "if key is object,key_hash_fn and key_equals_fn must be specified", 65);
        return AT_FALSE;
    }
    if (map->size >= map->threshold) {
        if (!expand_map(map)) {
	    return AT_FALSE;
	}
    }
    unsigned long index = key_hashcode(map, key, map->capcity);
    at_node_t *node_ptr = &map->buckets[index], *head = node_ptr;
    if (node_ptr->is_inuse) {
        while (node_ptr) {
            if (is_equals(map, &node_ptr->key, key)) {
                set_entry(&node_ptr->value, map->val_type, value);
                return AT_TRUE;
            }
            node_ptr = node_ptr->next;
        }
        node_ptr = (at_node_t *)malloc(sizeof(at_node_t));
        node_ptr->prev = head;
        node_ptr->next = head->next;
        if (head->next) {
            head->next->prev = node_ptr;
        }
        head->next = node_ptr;
    }
    set_entry(&node_ptr->key, map->key_type, key);
    set_entry(&node_ptr->value, map->val_type, value);
    node_ptr->is_inuse = AT_TRUE;
    map->size++;
    return AT_TRUE;
}

at_map_iterator_t hashmap_iterate(at_hashmap_t *map) {
    at_map_iterator_t itr = {0};
    int i = 0;
    at_node_t *node = next_valid_bucket(map, &i);
    itr.next_node = node;
    itr.bucket_index = i;
    return itr;
}

at_map_iterator_t *hashmap_next(at_hashmap_t *map, at_map_iterator_t *primer_itr) {
    if (primer_itr->next_node) {
        at_node_t *node = primer_itr->next_node;
        primer_itr->key = get_entry(&node->key, map->key_type);
        primer_itr->value = get_entry(&node->value, map->val_type);
        if (node->next) {
            primer_itr->next_node = node->next;
        } else {
            int i = primer_itr->bucket_index + 1;
            node = next_valid_bucket(map, &i);
            primer_itr->next_node = node;
            primer_itr->bucket_index = i;
        }
    } else {
        memset(primer_itr, 0, sizeof(at_map_iterator_t));
        return NULL;
    }
    return primer_itr;
}

void *hashmap_get(at_hashmap_t *map, void *key) {
    unsigned long index = key_hashcode(map, key, map->capcity);
    at_node_t *node_ptr = &map->buckets[index];
    if (!node_ptr->is_inuse) {
        return NULL;
    } else {
        while (node_ptr) {
            if (is_equals(map, &node_ptr->key, key)) {
                return get_entry(&node_ptr->value, map->val_type);
            }
            node_ptr = node_ptr->next;
        }
    }
    return NULL;
}

at_boolean_t hashmap_remove(at_hashmap_t *map, void *key) {
    unsigned long index = key_hashcode(map, key, map->capcity);
    at_node_t *node_ptr = &map->buckets[index];
    if (!node_ptr->is_inuse) {
        return AT_TRUE;
    } else if (is_equals(map, &node_ptr->key, key)) {
	//free_node(map, node_ptr);
        if (node_ptr->next) {
            node_ptr->key = node_ptr->next->key;
            node_ptr->value = node_ptr->next->value;
            if (node_ptr->next->next) {
                node_ptr->next->next->prev = node_ptr;
            }
            node_ptr->next = node_ptr->next->next;
            free(node_ptr->next);
        } else {
            node_ptr->is_inuse = AT_FALSE;
        }
        map->size--;
    } else {
        node_ptr = node_ptr->next;
        while (node_ptr) {
            if (is_equals(map, &node_ptr->key, key)) {
                node_ptr->prev->next = node_ptr->next;
                if (node_ptr->next) {
                    node_ptr->next->prev = node_ptr->prev;
                }
                //free_node(map, node_ptr);
                free(node_ptr);
                map->size--;
                break;
            }
        }
    }
    return AT_TRUE;
}

const char *hashmap_error() {
    return error_msg;
}

int hashmap_size(at_hashmap_t *map) {
    return map->size;
}

void hashmap_free(at_hashmap_t **map_ptr) {
    at_hashmap_t *map = *map_ptr;
    if (map) {
        int i = 0;
        at_node_t *ptr = NULL;
        for (i = 0; i < map->capcity; i++) {
            ptr = &map->buckets[i];
            if (ptr->is_inuse) {
                at_boolean_t in_maintable = AT_TRUE;
                while (ptr) {
                    //free_node(map, ptr);
                    if (!in_maintable) {
                        free(ptr);
                    } else {
                        in_maintable = AT_FALSE;
                    }
                    ptr = ptr->next;
                }
            }
        }
        free(map->buckets);
        map->buckets = NULL;
        free(map);
        *map_ptr = NULL;
    }
}
