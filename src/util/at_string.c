#include <string.h>
#include <stdlib.h>

#include  "util/at_string.h"

static at_string_t *new_one(int capcity, int cursor) {
    at_string_t *new_str = (at_string_t *)malloc(sizeof(at_string_t));
    new_str->size = cursor;
    new_str->capcity = capcity;
    new_str->buffer = (char *)calloc(capcity, 1);
    return new_str;
}

static void expand_buffer (at_string_t *str, int new_cap) {
    str->buffer = realloc(str->buffer, new_cap);
    memset(str->buffer + str->size, 0, new_cap - str->capcity);
    str->capcity = new_cap;
}

at_string_t *string_new(const char *ori) {
    if (!ori) {
        return string_blank(DEFAULT_BUFFER_SIZE);
    }
    int temp = strlen(ori);
    at_string_t *str = new_one(temp + 1, temp);
    memcpy(str->buffer, ori, temp);
    return str;
}

at_string_t *string_blank(int capcity) {
    return new_one(capcity, 0);
}

at_string_t *string_append(at_string_t *str, const char *to_append, size_t append_len) {
    int temp = append_len + str->size + 1;
    if (temp > str->capcity) {
        int temp2 = BUFFER_INCREASE_RATIO * str->capcity;
        int new_cap = temp > temp2 ? temp : temp2;
        expand_buffer (str, new_cap);
    }
    memcpy(str->buffer + str->size, to_append, append_len);
    str->size += append_len;
    return str;
}

at_string_t *string_appendch (at_string_t *str, char ch) {
    int new_cap = str->size + 2;
    if (new_cap > str->capcity) {
        new_cap = BUFFER_INCREASE_RATIO * str->capcity;
        expand_buffer (str, new_cap);
    }
    *(str->buffer + str->size) = ch;
    str->size++;
    return str;
}

int string_length (at_string_t *str) {
    return str->size;
}

char *string_cstr(at_string_t *str) {
    return str->buffer;
}

at_string_t *string_reset(at_string_t *str) {
    if (str->size > 0) {
        memset(str->buffer, 0, str->size);
        str->size = 0;
    }
    return str;
}

void string_free(at_string_t **str_ptr) {
    at_string_t *str = *str_ptr;
    if (str) {
        free(str->buffer);
        free(str);
        *str_ptr = NULL;
    }
}
