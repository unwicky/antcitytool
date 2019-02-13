#include <stdio.h>
#include <string.h>

#include "util/at_hashmap.h"

typedef struct {
    int pos;
    char name[1024];
} Node;

static char error_msg[64];

int main(int argc, char *argv[]) {
    int a;
    /*printf("enter any number to start:");
    fscanf(stdin, "%d", &a);*/
    at_hashmap_t *map = hashmap_new(78);
    int i = 0;
    for (i = 0; i < 50; i++) {
	    char str[10] = {0};
	    snprintf(str, 10, "hello%d", i);
        if (!hashmap_insert(map, STRINGENTRY(str), STRINGENTRY(str))) {
            printf("%s\n", hashmap_error());
        }
    }
    at_map_iterator_t primer_itr = hashmap_iterate(map);
    at_map_iterator_t *itr = NULL;
    while ((itr = hashmap_next(map, &primer_itr)) != NULL) {
        printf("key: %s, value: %s\n", itr->key->string_entry, 
            itr->value->string_entry);
    }
    /*printf("capcity: %d, inMainTable: %d, inList: %d\n", map->capcity, 
        map->in_maintable, map->in_list);
    printf("enter any number to free:");
    fscanf(stdin, "%d", &a);
    for (i = 0; i < 50; i++) {
	    char str[10] = {0};
	    snprintf(str, 10, "hello%d", i);
        at_entry_t *ret = hashmap_get(map, STRINGENTRY(str));
        if (ret) {
            printf("key: %s, ret: %s\n", str, ret->string_entry);
        }
    }
    hashmap_free(&map);
    printf("enter any number to quit:");
    fscanf(stdin, "%d", &a);*/
    /*char str[10] = {0};
    snprintf(str, 10, "hello29");
    at_entry_t key = STRINGENTRY(str), value = STRINGENTRY(str);
    MapNode *newNode = (MapNode *)malloc(sizeof(MapNode));
    newNode->key = key;
    newNode->value = value;
    printf("ketType:%d, key: %s, value: %s\n", newNode->key.type, 
        newNode->key.string_entry, newNode->value.string_entry);*/
    return 0;
}
