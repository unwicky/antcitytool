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
    at_hashset_t *set = hashset_new(32, STRINGTYPE);
    int i = 0;
    for (i = 0; i < 1000; i++) {
	char str[10] = {0};
	snprintf(str, 10, "hello%d", i);
        if (!hashset_insert(set, str)) {
            printf("%s\n", hashset_error());
        }
    }
    printf("capcity: %d\n", set->capcity);
    for (i = 996; i < 1010; i++) {
        char str[10] = {0};
	snprintf(str, 10, "hello%d", i);
        at_boolean_t ret = hashset_iscontain(set, str);
        if (ret) {
            printf("%s exist\n", str);
        } else {
            printf("%s not exist\n", str);
        }
    }
    hashset_free(&set);
    return 0;
}
