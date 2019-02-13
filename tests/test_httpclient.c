#include <stdio.h>

#include "http/at_httpclient.h"

void test_head_request(at_httpclient_t *client) {
    at_httpresponse_t *response = httpclient_head(client, 
        "http://f10.eastmoney.com/f10_v2/CompanySurvey.aspx?code=300657");
    struct curl_slist *cookies, *cursor;
    if (response->op_code == 0 && response->http_status) {
        char cookies[1024];
        printf("%s\n", string_cstr(response->header));
        at_boolean_t ret = httpclient_get_header(client, "Set-Cookie", cookies, 1023);
        if (ret) {
            printf("Set-Cookie=%s\n", cookies);
        } else {
            printf("Set-Cookie is not found\n");
        }
    } else {
        printf("%d-%d\n", response->op_code, response->http_status);
    }
}

void test_get_request(at_httpclient_t *client) {
    at_httpresponse_t *response = httpclient_get(client, "http://f10.eastmoney.com/f10_v2/CompanySurvey.aspx?code=sz300657");
    long lmt = 0;
    if (response->op_code == 0 && response->http_status) {
        printf("%s\n", string_cstr(response->ret_str));
    } else {
        printf("%d-%d\n", response->op_code, response->http_status);
    }
}

int main(int argc, char *argv[]) {
    at_httpclient_t *client = httpclient_new();
    //setProxy(httpClient, "127.0.0.1", 8087);
    //test_head_request(client);
    test_get_request(client);
    return 0;
}
