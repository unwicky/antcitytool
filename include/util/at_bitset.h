/*
 * at_bitset_t.h
 *
 *  Created on: 2014-9-24
 *      Author: herengao
 */

#ifndef AT_BITSET_H_
#define AT_BITSET_H_

typedef struct {
    unsigned long int *set;
    unsigned int group_capcity;
    unsigned int capcity;
    unsigned int size;
} at_bitset_t;

#ifdef __cplusplus
extern "C" {
#endif

at_bitset_t *bitset_new(unsigned int);

int bitset_set(at_bitset_t *, unsigned int);

int bitset_reset(at_bitset_t *, unsigned int);

void bitset_clear (at_bitset_t *);

int bitset_isset(at_bitset_t *, unsigned int);

int bitset_isallset(at_bitset_t *);

unsigned int bitset_count(at_bitset_t *);

unsigned int bitset_capcity(at_bitset_t *);

void bitset_free(at_bitset_t **);

#ifdef __cplusplus
}
#endif

#endif /* AT_BITSET_H_ */
