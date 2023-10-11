#ifndef FLOAT_TEST
#define FLOAT_TEST

#include <stdio.h>

typedef float key_type;

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
    printf("%f", key);
}

key_type default_key() {
    return .0;
}

#endif