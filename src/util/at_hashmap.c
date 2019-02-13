#include <stdio.h>

#include "util/at_hashmap.h"
#include "util/at_util.h"

static char error_msg[128];

static unsigned long key_hashcode(at_entry_t *key, unsigned int mask) {
    unsigned long hash = 1;
    switch (key->type) {
    case LONGTYPE:
        hash = (unsigned long)key->long_entry;
        break;
    case DOUBLETYPE:
	    hash = double_hashcode(key->double_entry);
        break;
    case STRINGTYPE:
        hash = simple_hash(key->string_entry);
        break;
    case OBJECTTYPE:
        if (key->hash_func) {
            hash = key->hash_func(key->object_entry);
        }
        break;
    default:
        hash = 1;
        break;
    }
    return hash & (mask - 1);
}

static at_boolean_t is_equals(at_entry_t* key1, at_entry_t* key2) {
    if (key1->type != key2->type) {
        return AT_FALSE;
    }
    switch (key1->type) {
    case LONGTYPE:
        return key1->long_entry == key2->long_entry;
    case DOUBLETYPE:
        return key1->double_entry == key2->double_entry;
    case STRINGTYPE:
        return strcmp(key1->string_entry, key2->string_entry) == 0;
    case OBJECTTYPE:
        if (key1->equals_func) {
            return key1->equals_func(key1->object_entry, key2->object_entry);
        } else {
            return AT_FALSE;
        }
    default:
        return AT_FALSE;
    }
}

static void free_entry(at_entry_t *entry) {
    if (entry->type == STRINGTYPE) {
        free(entry->string_entry);
    } else if (entry->type == OBJECTTYPE) {   
        if (entry->free_func) {
	        entry->free_func(entry->object_entry);
        } else {
            free(entry->object_entry);
	    }
    }
}

static void free_node(at_node_t *node) {
    free_entry(&node->key);
    free_entry(&node->value);
}

static at_boolean_t rehash(at_node_t *node, at_node_t *node_array, int hash_mask, 
	int *new_in_table, int *new_in_list) {
    unsigned int newIndex =  key_hashcode(&node->key, hash_mask);
    at_node_t *ptr = &node_array[newIndex];
    if (ptr->key.type == UNKNOWTYPE) {
	    ptr->key = node->key;
        ptr->value = node->value;
        (*new_in_table)++;
    } else {
        at_node_t *new_node = (at_node_t *)malloc(sizeof(at_node_t));
        if (!new_node) {
	        return AT_FALSE;
	    }
        new_node->key = node->key;
        new_node->value = node->value;
	    new_node->prev = ptr;
        new_node->next = ptr->next;
        if (ptr->next) {
	        ptr->next->prev = new_node;
	    }
	    ptr->next = new_node;
	    (*new_in_list)++;
    }
    return AT_TRUE;
}

static at_boolean_t expand_map(at_hashmap_t *map) {
    at_boolean_t ret = AT_TRUE;
    int new_capcity = map->capcity << 1, new_in_main_table = 0, new_in_list = 0, count = 0, i = 0;
    at_node_t *new_array = (at_node_t *)calloc(new_capcity, sizeof(at_node_t));
    if (!new_array) {
        strncpy(error_msg, "expand map failed:out of memory", 51);
	    return AT_FALSE;
    }
    for (i = 0; count < map->size && i < map->capcity; i++) {
        at_node_t *ptr1 = &map->array[i], *ptr2 = NULL;
        if(ptr1->key.type != UNKNOWTYPE) {
	        at_boolean_t is_in_list = AT_FALSE;
	        while (ptr1) {
	            ret = rehash(ptr1, new_array, new_capcity, &new_in_main_table, &new_in_list);
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
    free(map->array);
    map->array = new_array;
    map->capcity = new_capcity;
    map->threshold = new_capcity * map->load_factor;
    map->in_maintable = new_in_main_table;
    map->in_list = new_in_list;
    return AT_TRUE;
}

static at_node_t *next_valid_bucket(at_hashmap_t *map, int *bucket_index) {
    at_node_t *node_itr = NULL;
    int i = *bucket_index;
    for (; i < map->capcity; i++) {
        if (map->array[i].key.type != UNKNOWTYPE) {
            node_itr = &map->array[i];
            *bucket_index = i;
            break;
        }
    }
    return node_itr;
}

void hashmap_dump(at_hashmap_t *map) {
    int count = 0, i = 0; 
    for (i = 0; i < map->capcity; i++) {
        at_node_t *ptr = &map->array[i];
        if (ptr->key.type != UNKNOWTYPE) {
            while (ptr) {
                printf("key: %s, value: %s\n", ptr->key.string_entry, ptr->value.string_entry);
                ptr = ptr->next;
            }
        }
    }
}

at_hashmap_t *hashmap_new(int capcity) {
    if (capcity <= 0) {
        capcity = DEFAULT_MAP_CAPCITY;
    }
    at_hashmap_t *map = (at_hashmap_t *)malloc(sizeof(at_hashmap_t));
    map->capcity = min2n(capcity);
    map->array = (at_node_t *)calloc(map->capcity, sizeof(at_node_t));
    map->key_type = UNKNOWTYPE;
    map->value_type = UNKNOWTYPE;
    map->load_factor = DEFAULT_LOAD_FACTOR;
    map->size = 0;
    map->threshold = map->load_factor * map->capcity;
    map->in_maintable = 0;
    map->in_list = 0;
    return map;
}

void hashmap_setloadfactor(at_hashmap_t *map, double load_factor) {
    if (load_factor <= 0) {
        map->load_factor = load_factor;
        map->threshold = map->load_factor * map->capcity;
    }
}

at_boolean_t hashmap_insert(at_hashmap_t *map, at_entry_t key, at_entry_t value) {
    if (key.type == ILLEGALTYPE) {
    	strncpy(error_msg, "key type is illegal:char *(not NULL),long/int, double/float allowed", 67); 
        return AT_FALSE;
    }
    if (value.type == ILLEGALTYPE) {
	    strncpy(error_msg, "value type is illegal:char *(not NULL),long/int, double/float, void *(not NULL) allowed", 87); 
        return AT_FALSE;
    }
    memset(error_msg, 0, 64);
    if (map->key_type == UNKNOWTYPE) {
        map->key_type = key.type;
    } else if (key.type != map->key_type) {
        snprintf(error_msg, 64, "key type is incompatible,%d is valid", map->key_type);
        return AT_FALSE;
    } else if (map->key_type == OBJECTTYPE) {
        if (!key.hash_func) {
            snprintf(error_msg, 64, "hash_func can't be NULL for key with object type");
            return AT_FALSE;
        } else if (!key.equals_func) {
            snprintf(error_msg, 64, "equals_func can't be NULL for key with object type");
            return AT_FALSE;
        }
    }
    if (map->value_type == UNKNOWTYPE) {
        map->value_type = value.type;
    } else if (value.type != map->value_type) {
        snprintf(error_msg, 64, "value type is incompatible,%d is valid", map->value_type);
        return AT_FALSE;
    }
    if (map->size >= map->threshold) {
        if (!expand_map(map)) {
	        return AT_FALSE;
	    }
    }
    unsigned long index = key_hashcode(&key, map->capcity);
    at_node_t *node_ptr = &map->array[index], *head = node_ptr;
    if (node_ptr->key.type != UNKNOWTYPE) {
        while (node_ptr) {
            if (is_equals(&node_ptr->key, &key)) {
                node_ptr->value = value;
                return AT_TRUE;
            }
            node_ptr = node_ptr->next;
        }
        node_ptr = (at_node_t *)malloc(sizeof(at_node_t));
        map->in_list++;
        node_ptr->prev = head;
        node_ptr->next = head->next;
        if (head->next) {
            head->next->prev = node_ptr;
        }
        head->next = node_ptr;
    } else {
        map->in_maintable++;
    }
    node_ptr->key = key;
    node_ptr->value = value;
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
        primer_itr->key = &node->key;
        primer_itr->value = &node->value;
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

at_entry_t *hashmap_get(at_hashmap_t *map, at_entry_t key) {
    if (key.type == ILLEGALTYPE || key.type == UNKNOWTYPE || key.type != map->key_type) {
        return NULL;
    }
    unsigned long index = key_hashcode(&key, map->capcity);
    at_node_t *node_ptr = &map->array[index];
    if (node_ptr->key.type == UNKNOWTYPE) {
        return NULL;
    } else {
        while (node_ptr) {
            if (is_equals(&node_ptr->key, &key)) {
                return &node_ptr->value;
            }
            node_ptr = node_ptr->next;
        }
    }
    return NULL;
}

at_boolean_t hashmap_remove(at_hashmap_t *map, at_entry_t key) {
    if (key.type == ILLEGALTYPE) {
        return AT_FALSE;
    } else if (key.type != map->key_type) {
        snprintf(error_msg, 64, "key type is incompatible,%d is valid", map->key_type);
        return AT_FALSE;
    }
    unsigned long index = key_hashcode(&key, map->capcity);
    at_node_t *node_ptr = &map->array[index];
    if (node_ptr->key.type == UNKNOWTYPE) {
        return AT_TRUE;
    } else if (is_equals(&key, &node_ptr->key)) {
	    free_node(node_ptr);
        node_ptr->key.type = UNKNOWTYPE;
        map->size--;
    } else {
        node_ptr = node_ptr->next;
        while (node_ptr) {
            if (is_equals(&key, &node_ptr->key)) {
                node_ptr->prev->next = node_ptr->next;
                if (node_ptr->next) {
                    node_ptr->next->prev = node_ptr->prev;
                }
                free_node(node_ptr);
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
            ptr = &map->array[i];
            if (ptr->key.type != UNKNOWTYPE) {
                at_boolean_t in_maintable = AT_TRUE;
                while (ptr) {
                    free_node(ptr);
                    if (!in_maintable) {
                        free(ptr);
                    } else {
                        in_maintable = AT_FALSE;
                    }
                    ptr = ptr->next;
                }
            }
        }
        free(map->array);
        map->array = NULL;
        *map_ptr = NULL;
    }
}
