/*
 * at_httpclient_pool.c
 *
 *  Created on: 2016年5月13日
 *      Author: rengaohe
 */

#include <stdlib.h>
#include <curl/curl.h>

#include "http/at_httpclient_pool.h"

at_httpclient_pool_t *httpclient_pool_create (int pool_size) {
    int i = 0;
    at_httpclient_pool_t *pool =
            (at_httpclient_pool_t *)malloc (sizeof (at_httpclient_pool_t));
    pool->set = stack_new(pool_size);
    for (i = 0; i < pool_size; i++) {
        at_httpclient_t *client = httpclient_new ();
        //httpclient_set_proxy (client, "127.0.0.1", 8087);
        httpclient_set_timeout(client, 180L);
        curl_easy_setopt(client->handler, CURLOPT_NOSIGNAL, 1L);
        stack_push(pool->set, client);
    }
    pthread_mutex_init(&pool->lock, NULL);
    return pool;
}

at_httpclient_t *httpclient_pool_get (at_httpclient_pool_t *pool) {
    pthread_mutex_lock(&pool->lock);
    at_httpclient_t *client = (at_httpclient_t *)stack_pop(pool->set);
    pthread_mutex_unlock(&pool->lock);
    return client;
}

void httpclient_pool_return (at_httpclient_pool_t *pool,
        at_httpclient_t *client) {
    pthread_mutex_lock(&pool->lock);
    stack_push(pool->set, client);
    pthread_mutex_unlock(&pool->lock);
}

void httpclient_pool_destroy (at_httpclient_pool_t **pool_ptr) {
    at_httpclient_pool_t *pool = *pool_ptr;
    if (pool) {
        at_httpclient_t *client = NULL;
        while ((client = (at_httpclient_t *)stack_pop(pool->set)) != NULL) {
            httpclient_destroy(&client);
        }
        stack_free(&pool->set);
        pthread_mutex_destroy(&pool->lock);
        free (pool);
        *pool_ptr = NULL;
    }
}
