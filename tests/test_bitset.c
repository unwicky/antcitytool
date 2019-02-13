#include <stdio.h>

#include "util/at_bitset.h"

int main(int argc, char *argv[]) {
    at_bitset_t *bitset = bitset_new(1000);
    int ret = bitset_set(bitset, 199);
    printf("bitset_set: 199-%d\n", ret);
    ret = bitset_set(bitset, 189);
    printf("bitset_set: 189-%d\n", ret);
    ret = bitset_isset(bitset, 188);
    printf("bitset_isset:188-%d\n", ret);
    ret = bitset_isset(bitset, 199);
    printf("bitset_isset:199-%d\n", ret);
    ret = bitset_reset(bitset, 199);
    printf("rebitset_set:199-%d\n", ret);
    ret = bitset_isset(bitset, 199);
    printf("bitset_isset:199-%d\n", ret);
    bitset_free(&bitset);
    return 0;
}

