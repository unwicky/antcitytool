#ifndef AT_HTTPCLIENT_H_
#define AT_HTTPCLIENT_H_

#include <curl/curl.h>

#include "at_types.h"
#include "util/at_string.h"

#define HTTP_OK 200

typedef struct {
    int op_code;
    int http_status;
    at_string_t *header;
    at_string_t *ret_str;
} at_httpresponse_t;

typedef struct {
    CURL *handler;
    struct curl_slist *header_list;
    at_httpresponse_t *response;
} at_httpclient_t;

#ifdef __cplusplus
extern "C" {
#endif

at_httpclient_t *httpclient_new();

void httpclient_set_timeout(at_httpclient_t *, long timeout);

void httpclient_set_proxy(at_httpclient_t *, const char *, int);

void httpclient_remove_signal(at_httpclient_t *); 

void httpclient_reset_header(at_httpclient_t *);

void httpclient_set_header(at_httpclient_t *, const char *);

at_boolean_t httpclient_get_header(at_httpclient_t *, const char *, char *, int);

at_httpresponse_t *httpclient_head(at_httpclient_t *, const char *);

at_httpresponse_t *httpclient_get(at_httpclient_t *, const char *);

at_httpresponse_t *httpclient_post(at_httpclient_t *, const char *, const char **, int);

void httpclient_destroy(at_httpclient_t **);

#ifdef __cplusplus
}
#endif

#endif /* AT_HTTPCLIENT_H_ */
