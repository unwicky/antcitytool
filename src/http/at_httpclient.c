#include <stdlib.h>

#include "http/at_httpclient.h"

static size_t  on_receive (char *ret_str, size_t size, size_t nmemb, void *user_data) {
    size_t total_size = size * nmemb;
    if (total_size > 0) {
        at_string_t *response = (at_string_t *)user_data;
        string_append(response, ret_str, total_size);
    }
    return total_size;
}

static size_t on_receive_header(char *ret_str, size_t size, size_t nmemb, 
    void *header_data) {
    size_t total_size = size * nmemb;
    if (total_size > 0) {
        at_string_t *header = (at_string_t *)header_data;
        string_append(header, ret_str, total_size);
    }
    return total_size;
}

at_httpclient_t *httpclient_new() {
    CURL *handler = curl_easy_init();
    if (!handler) {
        return NULL;
    }
    at_httpclient_t *client = (at_httpclient_t *)malloc(sizeof(at_httpclient_t));
    client->response = (at_httpresponse_t *)malloc(sizeof(at_httpresponse_t));
    client->response->op_code = 0;
    client->response->http_status = HTTP_OK;
    client->response->ret_str = string_blank(100);
    client->response->header = string_blank(64);
    client->header_list = NULL;
//    curl_easy_setopt(handler, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(handler, CURLOPT_USERAGENT, 
        "Mozilla/5.0 (X11; U; Linux i686; zh-CN; rv:1.9.1.2) Gecko/20090803");
    curl_easy_setopt(handler, CURLOPT_TIMEOUT, 3L);
    curl_easy_setopt(handler, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handler, CURLOPT_WRITEFUNCTION, on_receive);
    curl_easy_setopt(handler, CURLOPT_WRITEDATA, client->response->ret_str);
    curl_easy_setopt(handler, CURLOPT_HEADERFUNCTION, on_receive_header);
    curl_easy_setopt(handler, CURLOPT_HEADERDATA, client->response->header);
    client->handler = handler;
    return client;
}

void httpclient_set_timeout(at_httpclient_t *client, long timeout) {
    curl_easy_setopt(client->handler, CURLOPT_TIMEOUT, timeout);
}

void httpclient_set_proxy(at_httpclient_t *client, const char *ip, int port) {
    curl_easy_setopt(client->handler, CURLOPT_PROXY, ip);
    curl_easy_setopt(client->handler, CURLOPT_PROXYPORT, port);
}

void httpclient_remove_signal(at_httpclient_t *client) {
    curl_easy_setopt(client->handler, CURLOPT_NOSIGNAL, 1);
}

void httpclient_reset_header(at_httpclient_t *client) {
    curl_slist_free_all(client->header_list);
    client->header_list = NULL;
}

void httpclient_set_header(at_httpclient_t *client, const char *header_opt) {
    client->header_list = curl_slist_append(client->header_list, header_opt);
}

at_boolean_t httpclient_get_header(at_httpclient_t *client, const char *key, 
    char *value, int limit_len) {
    char *header = string_cstr(client->response->header);
    const char *key_cursor = NULL;
    char *header_cursor = NULL;
    int len = 0;
    at_boolean_t found = AT_FALSE;
    while (*header != '\0') {
        if (*header == *key) {
            key_cursor = key;
            header_cursor = header;
            found = AT_TRUE;
            while (*key_cursor != '\0') {
                if (*header_cursor++ != *key_cursor++) {
                    found = AT_FALSE;
                    break;
                }
            }
            if (found) {
                for (; *header_cursor == ' ' || *header_cursor == ':'; 
                    header_cursor++);
                len = 0;
                while (*header_cursor != '\r' && *header_cursor != '\n') {
                    *value++ = *header_cursor++; 
                    if (++len >= limit_len) {
                        break;
                    }
                }
                *value = '\0';
                break;
            }
        }
        header++;
    }
    return found;
}

static void http_request(at_httpclient_t *client, const char *url) {
    string_reset(client->response->header);
    string_reset(client->response->ret_str);
    if (client->header_list) {
        curl_easy_setopt(client->handler, CURLOPT_HTTPHEADER, client->header_list);
    }
    curl_easy_setopt(client->handler, CURLOPT_URL, url);
    CURLcode ret_code = curl_easy_perform(client->handler);
    client->response->op_code = ret_code;
    if (ret_code == CURLE_OK) {
        long http_status = 0;
        curl_easy_getinfo(client->handler, CURLINFO_RESPONSE_CODE, &http_status);
        client->response->http_status = http_status;
    }
}

at_httpresponse_t *httpclient_head(at_httpclient_t *client, const char *url) {
    curl_easy_setopt(client->handler, CURLOPT_NOBODY, 1L);
    http_request(client, url);
    return client->response;
}

at_httpresponse_t *httpclient_get(at_httpclient_t *client, const char *url) {
    http_request(client, url);
    return client->response;
}

at_httpresponse_t *httpclient_post(at_httpclient_t *client, const char *url, 
        const char **params, int param_num) {
    int i = 0;
    for (i = 0; i < param_num; i++) {
        curl_easy_setopt(client->handler, CURLOPT_POSTFIELDS, params[i]);
    }
    http_request(client, url);
    return client->response;
}

void httpclient_destroy(at_httpclient_t **client_ptr) {
    at_httpclient_t *client = *client_ptr;
    if (client) {
        if (client->header_list) {
            curl_slist_free_all(client->header_list);            
        }
        curl_easy_cleanup(client->handler);
        string_free(&client->response->header);
        string_free(&client->response->ret_str);
        free(client->response);
        free(client);
        *client_ptr = NULL;
    }
}

