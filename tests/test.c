#include <stdio.h>

void main() {
    int a = 5;
    int *a_ptr = &a;
    long b = (long)a;
    printf("%ld\n", b);
}
