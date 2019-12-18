/*
 * at_httpclient_pool.h
 *
 *  Created on: 2019-2-20
 *      Author: rengaohe
 */

#ifndef AT_HTTPCLIENT_POOL_H_
#define AT_HTTPCLIENT_POOL_H_

#include <pthread.h>

#include "util/at_stack.h"
#include "http/at_httpclient.h"

typedef struct {
    at_stack_t *set;
    pthread_mutex_t lock;
} at_httpclient_pool_t;

typedef struct {
    const char *proxy_host;
    int proxy_port;
    const char *ssl_verify;
} at_httpclient_pool_config_t;

at_httpclient_pool_t *httpclient_pool_create (int, at_httpclient_pool_config_t *);

at_httpclient_t *httpclient_pool_get (at_httpclient_pool_t *);

void httpclient_pool_return (at_httpclient_pool_t *, at_httpclient_t *);

void httpclient_pool_destroy (at_httpclient_pool_t **);

#endif /* AT_HTTPCLIENT_POOL_H_ */
