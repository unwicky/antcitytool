#include <stdio.h>
#include <string.h>

#include "util/at_hashset.h"

typedef struct {
    int pos;
    char name[1024];
} Node;

static char error_msg[64];

int main(int argc, char *argv[]) {
    int a;
    printf("enter any number to start:");
    fscanf(stdin, "%d", &a);
    at_hashset_t *map = hashset_new(32);
    int i = 0;
    for (i = 0; i < 1000; i++) {
	    char str[10] = {0};
	    snprintf(str, 10, "hello%d", i);
        if (!hashset_insert(map, STRINGENTRY(str))) {
            printf("%s\n", hashset_error());
        }
    }
    printf("capcity: %d, in_main_table: %d, inList: %d\n", map->capcity, 
        map->in_maintable, map->in_list);
    printf("enter any number to free:");
    fscanf(stdin, "%d", &a);
    for (i = 996; i < 1010; i++) {
	    char str[10] = {0};
	    snprintf(str, 10, "hello%d", i);
        at_boolean_t ret = hashset_iscontain(map, STRINGENTRY(str));
        if (ret) {
            printf("%s exist\n", str);
        } else {
            printf("%s not exist\n", str);
        }
    }
    hashset_free(&map);
    printf("enter any number to quit:");
    fscanf(stdin, "%d", &a);
    return 0;
}
