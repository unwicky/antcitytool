/*
 * bitset.c
 *
 *  Created on: 2014-9-24
 *      Author: herengao
 */
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "util/at_bitset.h"

static unsigned int uint_size = sizeof(unsigned long int);
static unsigned int group_size = sizeof(unsigned long int)*8;

/**
 * get the index of array of set that contain the bit that
 * indentified by m_index
 */
static int
group_index(unsigned int index) {
    return  index / group_size;
}

/**
 * create bitset with bits num of m_cap
 */
at_bitset_t *bitset_new(unsigned int capcity) {
    at_bitset_t *bitset;
    bitset = (at_bitset_t *)malloc(sizeof(at_bitset_t));
    bitset->capcity = capcity;
    bitset->group_capcity = group_index(capcity - 1) + 1;
    bitset->set = (unsigned long int *)
            calloc(bitset->group_capcity, uint_size);
    bitset->size = 0;
    return bitset;
}

/**
 * set bit of m_bitset at index m_index to be 1
 */
int bitset_set(at_bitset_t *bitset, unsigned int index) {
    int ret = 0;
    assert(index >= 0 && index < bitset->capcity);
    unsigned long int temp = 1;
    int group_idx = 0;
    temp <<= index;
    group_idx = group_index(index);
    if((bitset->set[group_idx] & temp) == 0) {
        bitset->set[group_idx] |= temp;
        bitset->size++;
    } else {
        ret = -1;
    }
    return ret;
}

/**
 * reset bit at index of m_index to be 0
 */
int bitset_reset(at_bitset_t *bitset, unsigned int index) {
    int ret = 0;
    assert(index > 0 && index < bitset->capcity);
    unsigned long int temp = 1;
    int group_idx = 0;
    temp <<= index;
    group_idx = group_index(index);
    if((bitset->set[group_idx] & temp) == 0) {
        ret = -1;
    } else {
        temp = 0;
        bitset->set[group_idx] &= temp;
        bitset->size--;
    }
    return ret;
}

void bitset_clear (at_bitset_t *bitset) {
    memset (bitset->set, 0, bitset->group_capcity * uint_size);
    bitset->size = 0;
}

/**
 * judge if the bit at index of m_index is 1
 */
int bitset_isset(at_bitset_t *bitset, unsigned int index) {
    int ret = 1;
    assert(index >= 0 && index < bitset->capcity);
    unsigned long int temp = 1;
    int group_idx = 0;
    temp <<= index;
    group_idx = group_index(index);
    if((bitset->set[group_idx] & temp) == 0) {
        ret = 0;
    }
    return ret;
}

/**
 * judge if all bits in bitset are setted to be 1
 */
int bitset_isallset(at_bitset_t *bitset) {
    int ret = 0;
    if(bitset->size == bitset->capcity) {
        ret = 1;
    }
    return ret;
}

/**
 * get the number of bits that have been setted to 1
 */
unsigned int bitset_count(at_bitset_t *bitset) {
    unsigned int count;
    count = bitset->size;
    return count;
}

/**
 * get the total number of bits of the bitset
 */
unsigned int bitset_capcity(at_bitset_t *bitset) {
    unsigned int cap;
    cap = bitset->capcity;
    return cap;
}

/**
 * free all memory that allocted to the bitset m_bitset
 */
void bitset_free(at_bitset_t **bitset_ptr) {
    at_bitset_t *bitset = *bitset_ptr;
    free(bitset->set);
    free(bitset);
    *bitset_ptr = NULL;
}
