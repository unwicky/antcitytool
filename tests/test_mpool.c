/*
 * test_palloc.c
 *
 *  Created on: 2016年3月22日
 *      Author: rengaohe
 */

#include <stdio.h>
#include <time.h>

#include "util/at_mpool.h"

int main(int argc, char *argv[]) {
    int cmd;
    int i = 0;
    time_t start, end;
    at_mpool_t *mpool = mpool_create(1024);
    char *a = mpool_malloc(mpool, 32);
    snprintf(a, 32, "hello,a");
    char *b = mpool_malloc(mpool, 16);
    snprintf(b, 32, "hello,b");
    char *c = mpool_malloc(mpool, 32);
    snprintf(c, 32, "hello,c");
    char *d = mpool_malloc(mpool, 32);
    snprintf(d, 32, "hello,d");
    mpool_free(mpool, (void **)&b);
    mpool_compress(mpool);
    printf("%s\n", c);
    mpool_destroy(&mpool);

    return 0;
}
