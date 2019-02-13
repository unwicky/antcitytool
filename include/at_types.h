/*
 * AntcityTypes.h
 *
 *  Created on: 2015-4-15
 *      Author: rengaohe
 */

#ifndef AT_TYPES_H_
#define AT_TYPES_H_

#include "at_define.h"

typedef enum {
    AT_FALSE = 0,
    AT_TRUE
} at_boolean_t;

typedef struct {
    int code;
    char error_msg[MAX_ERROR_MSG_LEN];
    void *obj;
} at_error_t;

#endif /* AT_TYPES_H_ */

