#include <stdio.h>

#include "at_types.h"
#include "util/at_util.h"
#include "util/at_string.h"

const static int ONE_DAY = 24*60*60;
const static int ONE_MONTH = 30*24*60*60;
const static int FIVE_YEAR = 5*12*30*24*60*60;

int main(int argc, char *argv[]) {
    const char *format1 = "%Y-%m-%d %H:%M:%S";
    const char *format2 = "%Y-%m-%d";
    const char *format3 = "%y%m%d";
    char *time_str1 = "990602";
    time_t t1 = str_to_time (time_str1, format3);
    printf ("%ld\n", t1);
    char time_str[11] = {0}, time_str2[20] = {0};
    time_to_str (t1, time_str, 11, format2);
    printf ("%s\n", time_str);
    return 0;
}
