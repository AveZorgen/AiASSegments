#ifndef INT_TEST
#define INT_TEST

#include <stdio.h>

typedef int key_type;

bool l(key_type a, key_type b) {
    return a < b;
}

bool eq(key_type a, key_type b) {
    return a == b;
}

bool le(key_type a, key_type b) {
    return l(a, b) || eq(a, b);
}

void print_key(key_type key) {
    printf("%d", key);
}

key_type default_key() {
    return 0;
}

#endif