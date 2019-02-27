#include <stdio.h>
#include <string.h>

#include "util/at_hashmap.h"

typedef struct {
    int id;
    char *name;
} person;

static char error_msg[64];

void test_get_int() {
    at_hashmap_t *map = hashmap_new(64, INTTYPE, INTTYPE);
    int i = 0;
    for (i = 0; i < 10; i++) {
        if (!hashmap_insert(map, &i, &i)) {
            printf("%s\n", hashmap_error());
        }
    }
    int key = 5;
    void *val5 = hashmap_get(map, &key);
    if (val5) {
        printf("%d\n", *(int *)val5);
    } else {
        printf("value is not exist\n");
    }
    hashmap_free(&map);
}

void test_remove() {
    at_hashmap_t *map = hashmap_new(64, INTTYPE, INTTYPE);
    int i = 0;
    for (i = 0; i < 10; i++) {
        if (!hashmap_insert(map, &i, &i)) {
            printf("%s\n", hashmap_error());
        }
    }
    int key = 5;
    hashmap_remove(map, &key);
    void *val5 = hashmap_get(map, &key);
    if (!val5) {
        printf("val5 is not exist\n");
    } else {
        printf("%d\n", *(int *)val5);
    }
    hashmap_free(&map);
}

void test_iterate() {
    at_hashmap_t *map = hashmap_new(64, STRINGTYPE, STRINGTYPE);
    int i = 0;
    char *val = NULL;
    hashmap_insert(map, "key1", "val1");
    hashmap_insert(map, "key2", "val2");
    at_map_iterator_t primer_itr = hashmap_iterate(map);
    at_map_iterator_t *itr = NULL;
    while ((itr = hashmap_next(map, &primer_itr)) != NULL) {
        val = (char *)itr->value;
        printf("key: %s, value: %s\n", (char *)itr->key, val);
    }
    hashmap_free(&map);
}

unsigned long hash_node(void *v) {
    person *node = (person *)v;
    return (unsigned long)node->id;
}

at_boolean_t equals_node(void *v1, void *v2) {
    person *node1 = (person *)v1;
    person *node2 = (person *)v2;
    if (node1->id == node2->id) {
        return AT_TRUE;
    } else {
        return AT_FALSE;
    }
}

/*void free_node(void *v) {
    person *node = (person *)v;
    free(node->name);
    free(node);
}*/

void test_complex_key() {
    at_hashmap_t *map = hashmap_new(64, OBJECTTYPE, INTTYPE);
    hashmap_set_key_hash_func(map, hash_node);
    hashmap_set_key_equals_func(map, equals_node);
    //hashmap_set_key_free_func(map, free_node);
    person *node = (person *)malloc(sizeof(person));
    node->name = (char *)malloc(64);
    snprintf(node->name, 64, "%s", "node1");
    node->id = 1;
    int val = 5;
    hashmap_insert(map, node, &val);
    person key = {0};
    key.id = 1;
    void *data = hashmap_get(map, &key);
    if (data) {
        printf("data is %d\n", *(int *)data);
    } else {
        printf("data is not exist\n");
    }
    free(node->name);
    free(node);
    hashmap_free(&map);
}

void test_complex_value() {
    at_hashmap_t *map = hashmap_new(64, INTTYPE, OBJECTTYPE);
    //hashmap_set_val_free_func(map, free_node);
    person *node = (person *)malloc(sizeof(person));
    node->name = (char *)malloc(64);
    snprintf(node->name, 64, "%s", "node1");
    node->id = 1;
    int key = 6;
    if (!hashmap_insert(map, &key, node)) {
        printf("%s\n", hashmap_error(map));
    } else {
        void *data = hashmap_get(map, &key);
        if (data) {
            person *nd = (person *)data;
            printf("id=%d,name=%s\n", nd->id, nd->name);
        } else {
            printf("data is not exist\n");
        }
    }
    free(node->name);
    free(node);
    hashmap_free(&map);
}

int main(int argc, char *argv[]) {
    printf("--------------get_int---------------\n");
    test_get_int();
    printf("--------------remove---------------\n");
    test_remove();
    printf("--------------iterate---------------\n");
    test_iterate();
    printf("--------------complex_key---------------\n");
    test_complex_key();
    printf("--------------complex_value---------------\n");
    test_complex_value();
    return 0;
}
