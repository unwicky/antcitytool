/*
 * at_mpool.h
 *
 *  Created on: 2016年3月22日
 *      Author: rengaohe
 */

#ifndef AT_MPOOL_H_
#define AT_MPOOL_H_

#include <stdlib.h>

typedef struct at_alloc_s at_alloc_t;
struct at_alloc_s {
    void *start;
    void *end;
    at_alloc_t *prev;
    at_alloc_t *next;
};

typedef struct {
    void *start;
    size_t total_free;
    at_alloc_t *allocs;
    at_alloc_t *last_alloc;
    void *end;
} at_mpool_t;

#define RESERVE_POOL_SIZE sizeof(at_mpool_t)
#define RESERVE_ALLOC_SIZE sizeof(at_alloc_t)

#ifdef __cplusplus
extern "C" {
#endif

at_mpool_t *mpool_create(size_t);

void *mpool_malloc(at_mpool_t *, size_t);

void *mpool_calloc(at_mpool_t *, size_t);

void mpool_compress(at_mpool_t *mpool);

void mpool_free(at_mpool_t *, void **);

void mpool_reset(at_mpool_t *);

void mpool_destroy(at_mpool_t **);

#ifdef __cplusplus
}
#endif

#endif /* AT_MPOOL_H_ */
