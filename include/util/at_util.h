/*
 * AntcityUtil.h
 *
 *  Created on: 2015年7月13日
 *      Author: herengao
 */

#ifndef AT_UTIL_H_
#define AT_UTIL_H_

#include <string.h>
#include <time.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

int try_mkdir (const char* dir);

char *get_file_ext(char *filename);

time_t str_to_time(const char *, const char *);

void time_to_str(time_t t, char *str_t, int len, const char *);

unsigned long onebyone_hash(const char *);

unsigned long simple_hash(const char *);

unsigned long double_hashcode(double);

unsigned int is2n(unsigned int);

unsigned int max2n(unsigned int);

unsigned int min2n(unsigned int);

int is_CN (char);

int is_full_char (char *);

int is_full_blank (char *);

int is_digit(const char *);

char *trim(char *, size_t);

#ifdef __cplusplus
}
#endif

#endif /* AT_UTIL_H_ */
