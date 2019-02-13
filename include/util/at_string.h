#ifndef AT_STRING_H_
#define AT_STRING_H_

#define DEFAULT_BUFFER_SIZE 16
#define BUFFER_INCREASE_RATIO 2

typedef struct {
    char *buffer;
    int size;
    int capcity;
} at_string_t;

#ifdef __cplusplus
extern "C" {
#endif

at_string_t *string_new(const char *);

at_string_t *string_blank(int );

at_string_t *string_append(at_string_t *, const char *, size_t);

at_string_t *string_appendch (at_string_t *, char);

int string_length (at_string_t *);

char *string_cstr(at_string_t *);

at_string_t *string_reset(at_string_t *);

void string_free(at_string_t **);

#ifdef __cplusplus
}
#endif

#endif /* AT_STRING_H_ */
