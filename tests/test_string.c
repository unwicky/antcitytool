#include <stdio.h>
#include <string.h>

#include "util/at_string.h"

int main(int argc, char *argv[]) {
    at_string_t *str = string_blank(100);
    at_string_t *temp = NULL;
    temp = string_append(str, "hello,at_string_t!", strlen("hello,at_string_t!"));
    printf("%s-%s\n", string_cstr(str), string_cstr(temp));
    temp = string_append(str, "today is fine!", strlen("today is fine!"));
    printf("%s-%s\n", string_cstr(str), string_cstr(temp));
    string_reset(str);
    printf("%s\n", string_cstr(str));
    temp = string_append(str, "new guys!", strlen("new guys!"));
    printf("%s-%s\n", string_cstr(str), string_cstr(temp));
    string_free(&str);
    str = string_new("hello!");
    printf("%s\n", string_cstr(str));
    string_appendch (str, 'h');
    string_appendch (str, 'r');
    string_appendch (str, 'g');
    printf("%s\n", string_cstr(str));
    string_free(&str);
    return 0;
}
