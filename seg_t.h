#ifndef SEG_TEST
#define SEG_TEST

#include <stdbool.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define EPS 1E-6

#define PNT_INFO int seg_idx; bool is_left;
typedef struct { double x; double y; PNT_INFO } Point, *pPoint;

#define SEG_INFO double k; double t_as_param;
typedef struct { Point u; Point v; SEG_INFO } Seg, *pSeg; 

inline void seg_set_k(Seg *seg) {
    seg->k = (seg->v.y - seg->u.y) / (seg->v.x - seg->u.x);
}

typedef pSeg key_type;

// common to tree keys functions

inline double intersect_with_sweep(Seg a, double t) {
    return a.u.y + a.k * (t - a.u.x);   
}

inline bool l(key_type a, key_type b, double t) {
    // if (a->u.x > t || a->v.x < t ||
    //    b->u.x > t || b->v.x < t) {
    //    printf("Boo! %i %i\n", a->u.seg_idx, b->u.seg_idx);
    // }
    return intersect_with_sweep(*a, t) < intersect_with_sweep(*b, t);
}

inline bool eq(key_type a, key_type b, double t) {
    return a == b;
}

inline bool le(key_type a, key_type b, double t) {
    return l(a, b, t) || fabs(intersect_with_sweep(*a, t) - intersect_with_sweep(*b, t)) < EPS;
    //return l(a, b, t);
}

inline void print_key(key_type key) {
    printf("%d: [(%lf %lf) (%lf %lf)]", key->u.seg_idx, key->u.x, key->u.y, key->v.x, key->v.y);
}

inline key_type default_key() {
    return NULL;
}

#endif