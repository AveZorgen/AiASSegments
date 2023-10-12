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
typedef struct { Point u; Point v; SEG_INFO } Seg, *pSeg, **ppSeg; 


inline bool double_intersect1d(double l1, double r1, double l2, double r2) {
    if (l1 > r1) {
        double t = l1;
        l1 = r1;
        r1 = t;
    }
    if (l2 > r2) {
        double t = l2;
        l2 = r2;
        r2 = t;
    }
    return fmax(l1, l2) <= fmin(r1, r2) + EPS;
}

inline int vec(Point a, Point b, Point c) {
    double s = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return fabs(s) < EPS ? 0 : s > 0 ? +1
                                     : -1;
}

inline bool intersect(pSeg s1, pSeg s2) {
    return double_intersect1d(s1->u.x, s1->v.x, s2->u.x, s2->v.x) &&
           double_intersect1d(s1->u.y, s1->v.y, s2->u.y, s2->v.y) &&
           vec(s1->u, s1->v, s2->u) * vec(s1->u, s1->v, s2->v) <= 0 &&
           vec(s2->u, s2->v, s1->u) * vec(s2->u, s2->v, s1->v) <= 0;
}


inline void seg_set_k(Seg *seg) {
    seg->k = (seg->v.y - seg->u.y) / (seg->v.x - seg->u.x);
}

inline double intersect_with_sweep(Seg a, double t) {
    return a.u.y + a.k * (t - a.u.x);   
}

// common to tree keys functions

typedef pSeg key_type;


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