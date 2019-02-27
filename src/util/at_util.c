#include <time.h>
#include <sys/stat.h>
#include <stdio.h>

#include "util/at_util.h"

inline int try_mkdir (const char* dir) {
	int ret = 0;
	if (access(dir, 0) != 0) {
		ret = mkdir(dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH);
	}
	return ret;
}

inline char *get_file_ext(char *fn) {
	char *ext = strrchr(fn, '.');
	if (ext) {
	    ext++;
	}
	return ext;
}

time_t str_to_time(const char *time_str, const char *format) {
    struct tm st = {0};
    time_t  t = 0;
    strptime(time_str, format, &st);
    t = mktime(&st);
    return t;
}

void time_to_str(time_t t, char *str_t, int len, const char *format) {
    struct tm *s_t;
    if (str_t) {
        s_t = localtime(&t);
        strftime(str_t, len, format, s_t);
    }
}

unsigned long onebyone_hash(const char *str) {
    if (!str) {
        return 0;
    }
    unsigned long ret = 0;
    int len = strlen(str), i = 0;
    for (i = 0; i < len; i++) {
        ret += str[i];
        ret += (ret << 10);
        ret ^= (ret >> 6);
    }
    ret += (ret << 3);
    ret ^= (ret >> 11);
    ret += (ret << 15);
    return ret;
}

unsigned long simple_hash(const char *str) {
    if (!str) {
        return 0;
    }
    unsigned long ret = 0;;
    int len = strlen(str), i = 0;
    for (i = 0; i < len; i++) {
        ret = (ret << 5) - ret + str[i];
    }
    return ret;
}

unsigned long double_hashcode(double d) {
    char temp[30] = {0};
    snprintf(temp, 30, "%.4f", d);
    return simple_hash(temp); 
}

/**
* if n is 2^m, return 0, else return x > 0
**/
unsigned int is2n(unsigned int n) {
    return n & (n - 1);
}

/**
* if n is 2^m, return n,else return x, where x maximum number that 
* is 2^k and x < n
**/
unsigned int max2n(unsigned int n) {
    if (!is2n(n)) {
        return n;
    }
    unsigned int ret = 1;
    while (n) {
        n >>= 1;
        if (n) {
            ret <<= 1;
        }
    }
    return ret;
}

/**
* if n is 2^m, return n,else return x, where x minmum number that 
* is 2^k and x > n
**/
unsigned int min2n(unsigned int n) {
    if (!n) {
        return 1;
    }
    if (!is2n(n)) {
        return n;
    }
    unsigned int ret = 1;
    while(n) {
        ret <<= 1;
        n>>=1;
    }
    return ret;
}

int is_CN (char ch) {
    if (ch & 0x80) {
        return 1;
    }
    return 0;
}

int is_full_char (char *str) {
    int cn_char = 0;
    if (is_CN(*str)) {
        cn_char = (str[0] & 0xF) << 12 | ((str[1] & 0x3F) << 6 | (str[2] & 0x3F));
        if (cn_char >= 0xFF01 && cn_char <= 0xFF5E) {
            //int en_char = cn_char - 0xFEE0;
            return 1;
        }
    }
    return 0;
}

int is_full_blank (char *str) {
    int cn_char = 0;
    if (is_CN(*str)) {
        cn_char = (str[0] & 0xF) << 12 | ((str[1] & 0x3F) << 6 | (str[2] & 0x3F));
        if (cn_char == 0x3000) {
            //int en_char = 0x20;
            return 1;
        }
    }
    return 0;
}

int is_digit(const char *str) {
    for (; *str == ' ' || *str == '\t';  str++);
    if (*str == '-' || *str == '+') {
        str++;
    }
    int dot_count = 0;
    while (*str != '\0') {
        if (*str == '.') {
            if (dot_count++ > 0) {
                return 0;
            }
        } else if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

char *trim(char *str, size_t len) {
    if (len <= 0) {
        return str;
    }
    int i = len - 1;
    while (i >= 0) {
        if (str[i] == ' ' || str[i] == '\t') {
            str[i] = '\0';
            len--;
            i--;
        } else if (is_CN(str[i]) && is_full_blank (&str[i - 2])) {
            str[i - 2] = '\0';
            len -= 3;
            i -= 3;
        } else {
            break;
        }
    }
    if (len > 0) {
        i = 0;
        while (i < len) {
            if (*str == ' ' || *str == '\t') {
                str++;
                len--;
                i++;
            } else if (is_CN (*str) && is_full_blank (str)) {
                str += 3;
                len -= 3;
                i += 3;
            } else {
                break;
            }
        }
    }
    if (len == 0) {
        str[0] = '\0';
    }
    return str;
}
