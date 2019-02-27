#include <stdio.h>
#include <string.h>

#include "util/at_hashset.h"

typedef struct {
    int pos;
    char name[1024];
} Node;

static char error_msg[64];

void test_iscontain() {
    int a;
    at_hashset_t *set = hashset_new(32, STRINGTYPE);
    int i = 0;
    char *data = NULL;
    for (i = 0; i < 1000; i++) {
	data = (char *)malloc(64);
	snprintf(data, 64, "hello%d", i);
        if (!hashset_insert(set, data)) {
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
    void **data_array = hashset_array(set), **head;
    void *itr = NULL;
    if (data_array) {
        head = data_array;
        while ((itr = *data_array) != NULL) {
            free(itr);
            data_array++;
        }
        free(head);
    }
    hashset_free(&set);
}

void test_array() {
    at_hashset_t *set = hashset_new(32, STRINGTYPE);
    int i = 0;
    char *data = NULL;
    for (i = 0; i < 20; i++) {
        data = (char *)malloc(64);
        snprintf(data, 64, "data%d", i);
        if (!hashset_insert(set, data)) {
            printf("%s\n", hashset_error());
        }
    }
    void **data_array = hashset_array(set), **head;
    void *temp = NULL;
    if (data_array) {
        head = data_array;
        while ((temp = *data_array) != NULL) {
            printf("%s\n", (char *)temp);
            free(temp);
            data_array++;
        }
        free(head);
    }
    hashset_free(&set);
}

int main(int argc, char *argv[]) {
    //printf("------------iscontain-------------\n");
    //test_iscontain();
    printf("------------array-------------\n");
    test_array();
    return 0;
}
