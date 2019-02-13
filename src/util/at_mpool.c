/*
 * at_mpool.c
 *
 *  Created on: 2016年3月22日
 *      Author: rengaohe
 */

#include <stdio.h>
#include <string.h>
#include "util/at_mpool.h"

static void mpool_move(at_alloc_t *dst, at_alloc_t **src_ptr) {
    at_alloc_t *src = *src_ptr;
    int offset = (void *)src - (void *)dst;
    dst->prev = src->prev;
    dst->start = src->start - offset;
    dst->end = src->end - offset;
    dst->next = src->next;
    memcpy(dst->start, src->start, src->end - src->start);
    *src_ptr = dst;
}

void mpool_compress(at_mpool_t *mpool) {
    at_alloc_t *cur = NULL;
    void *ori_data_pos = NULL, *new_data_pos = NULL;
    int offset = 0;
    if (mpool->allocs != mpool->start) {
        mpool_move((at_alloc_t *)mpool->start, &mpool->allocs);
    }
    cur = mpool->allocs;
    while (cur && cur->next) {
        if (cur->next != cur->end) {
            mpool_move((at_alloc_t *)cur->end, &cur->next);
        }
        cur = cur->next;
    }
    mpool->last_alloc = cur;
}

static void mpool_expand(at_mpool_t **mpool_ptr) {
    at_mpool_t *mpool = *mpool_ptr;
    at_alloc_t *cursor = mpool->allocs;
    size_t oldcap = mpool->end - mpool->start;
    size_t newcap = oldcap * 3 / 2 + 1;
    printf("%d,%d\n", oldcap, newcap);
    size_t new_real_size = newcap + RESERVE_POOL_SIZE, size = 0;
    at_mpool_t *p = mpool_create(new_real_size);
    void *new_ele = NULL;
    while (cursor) {
        size = cursor->end - cursor->start;
        new_ele = mpool_malloc(p, size);
        memcpy(new_ele, cursor->start, size);
        cursor = cursor->next;
    }
    free(mpool);
    *mpool_ptr = p;
}

at_mpool_t *mpool_create(size_t size) {
    at_mpool_t *p = NULL;
    size_t real_size = size + RESERVE_POOL_SIZE;
    p = (at_mpool_t *)malloc(real_size);
    p->start = (void *)p + RESERVE_POOL_SIZE;
    p->end = (void *)p + real_size;
    p->allocs = NULL;
    p->last_alloc = NULL;
    p->total_free = size;
    return p;
}

void *mpool_malloc(at_mpool_t *mpool, size_t size) {
    size_t real_size = size + RESERVE_ALLOC_SIZE;
    at_alloc_t *p = NULL;
    if (mpool->total_free < real_size) {
        mpool_expand(&mpool);
        return NULL;
    }
    if (mpool->last_alloc) {
        size_t tail_size = mpool->end - mpool->last_alloc->end;
        if (tail_size >= real_size) {
            p = mpool->last_alloc->end;
            p->prev = mpool->last_alloc;
            p->next = NULL;
            mpool->last_alloc->next = p;
            mpool->last_alloc = p;
        } else {
            at_alloc_t *cur = mpool->allocs;
            while (cur && cur->next) {
                if ((void *)cur->next - cur->end > real_size) {
                    p = cur->end;
                    p->prev = cur;
                    p->next = cur->next;
                    cur->next->prev = p;
                    cur->next = p;
                    break;
                }
                cur = cur->next;
            }
            if (!p) {
                mpool_compress(mpool);
                printf("fragment is too much\n");
                return NULL;
            }
        }
    } else {
        p = mpool->start;
        mpool->last_alloc = p;
        mpool->allocs = p;
        p->prev = NULL;
        p->next = NULL;
    }
    p->start = (void *)p + RESERVE_ALLOC_SIZE;
    p->end = (void *)p + real_size;
    mpool->total_free -= real_size;
    return p->start;
}

void *mpool_calloc(at_mpool_t *mpool, size_t size) {
    void *ptr = mpool_malloc(mpool, size);
    memset(ptr, 0, size);
    return ptr;
}

void mpool_free(at_mpool_t *mpool, void **data_ptr) {
    void *data = *data_ptr;
    at_alloc_t *p = (at_alloc_t *)(data - RESERVE_ALLOC_SIZE);
    at_alloc_t *prev = p->prev, *next = p->next;
    if (!prev && !next) {
        mpool->allocs = NULL;
        mpool->last_alloc = NULL;
    } else if (!prev) {
        mpool->allocs = next;
        next->prev = NULL;
    } else if (!next) {
        mpool->last_alloc = prev;
        prev->next = NULL;
    } else {
        prev->next = next;
        next->prev = prev;
    }
    mpool->total_free += (p->end - p->start + RESERVE_ALLOC_SIZE);
    *data_ptr = NULL;
}

void mpool_reset(at_mpool_t *mpool) {
    mpool->allocs = NULL;
    mpool->last_alloc = NULL;
    mpool->total_free = mpool->end - mpool->start;
}

void mpool_destroy(at_mpool_t **mpool_ptr) {
    at_mpool_t *mpool = *mpool_ptr;
    if (mpool) {
        free(mpool);
        *mpool_ptr = NULL;
    }
}

