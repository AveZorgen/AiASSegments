#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "seg_t.h"

bool double_intersect1d(double l1, double r1, double l2, double r2) {
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

int vec(Point a, Point b, Point c) {
    double s = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return fabs(s) < EPS ? 0 : s > 0 ? +1
                                     : -1;
}

bool intersect(pSeg s1, pSeg s2) {
    return double_intersect1d(s1->u.x, s1->v.x, s2->u.x, s2->v.x) &&
           double_intersect1d(s1->u.y, s1->v.y, s2->u.y, s2->v.y) &&
           vec(s1->u, s1->v, s2->u) * vec(s1->u, s1->v, s2->v) <= 0 &&
           vec(s2->u, s2->v, s1->u) * vec(s2->u, s2->v, s1->v) <= 0;
}

Seg random_uv_in_square(double width, int seg_idx) {
    return (Seg){
        .u = {.x = (double)rand() / RAND_MAX * width, .y = (double)rand() / RAND_MAX * width, .seg_idx = seg_idx, .is_left = true},
        .v = {.x = (double)rand() / RAND_MAX * width, .y = (double)rand() / RAND_MAX * width, .seg_idx = seg_idx, .is_left = false},
    };
}

void generate_random_segs(pSeg segs, int n) {
    double k;
    for (int i = 0; i < n; i++) {
        segs[i] = random_uv_in_square(1.0, i);
        k = (segs[i].v.y - segs[i].u.y) / (segs[i].v.x - segs[i].u.x);
        if (isinf(k))
            i--;
    }
}

void generate_random_segs_with_k(pSeg segs, int n, int k, double width) {
    int i = 0;
    bool ok;
    double coeff;
    for (; i < k + 1 && i < n; i++) {
        Seg tmp = random_uv_in_square(width, i);
        coeff = (tmp.v.y - tmp.u.y) / (tmp.v.x - tmp.u.x);
        if (isinf(coeff)) {
            i--;
            continue;
        }
        ok = true;
        for (int j = 0; j < i; j++) {
            if (intersect(&segs[j], &tmp)) {
                ok = false;
                i--;
                break;
            }
        }
        if (ok)
            segs[i] = tmp;
    }
    // seg[k]                       не пересекает seg[1..k-1]
    // seg[k+1] пересекает seg[k],  не пересекает seg[1..k-1]

    for (; i < k + 2 && i < n && i > 0; i++) {
        Seg tmp = random_uv_in_square(width, i);
        coeff = (tmp.v.y - tmp.u.y) / (tmp.v.x - tmp.u.x);
        if (!intersect(&segs[i - 1], &tmp) || isinf(coeff)) { // и не равны (f
            i--;
            continue;
        }
        ok = true;
        for (int j = 0; j < i - 1; j++) {
            if (intersect(&segs[j], &tmp)) {
                ok = false;
                i--;
                break;
            }
        }
        if (ok)
            segs[i] = tmp;
    }

    for (; i < n; i++) {
        Seg tmp = random_uv_in_square(width, i);
        coeff = (tmp.v.y - tmp.u.y) / (tmp.v.x - tmp.u.x);
        if (isinf(coeff)) {
            i--;
            continue;
        }
        ok = true;
        for (int j = 0; j < k; j++) {
            if (intersect(&segs[j], &tmp)) {
                ok = false;
                i--;
                break;
            }
        }
        if (ok)
            segs[i] = tmp;
    }
}

void generate_random_segs_in_square(pSeg segs, int n, double r) {
    double k;
    for (int i = 0; i < n; i++) {
        double center_x = (double)rand() / RAND_MAX;
        double center_y = (double)rand() / RAND_MAX;
        double alpha = M_PI * (double)rand() / RAND_MAX - M_PI_2;
        double dx = r * cos(alpha) / 2.0;
        double dy = r * sin(alpha) / 2.0;
        segs[i] = (Seg){
            .u = {.x = center_x - dx, .y = center_y - dy, .seg_idx = i, .is_left = true},
            .v = {.x = center_x + dx, .y = center_y + dy, .seg_idx = i, .is_left = false},
        };
        k = (segs[i].v.y - segs[i].u.y) / (segs[i].v.x - segs[i].u.x);
        if (isinf(k))
            i--;
    }
}

int main(int argc, char **argv) {
    int n = 5, k = 5, mode = 0;
    double r = 0.3, width = 1.0;
    unsigned int seed = 42;
    pSeg segs;

    int i = 1;
    sscanf(argv[i++], "%d", &mode);
    sscanf(argv[i++], "%d", &n);
    segs = malloc(n * sizeof(*segs));
    switch (mode) {
    case 0:
        if (i < argc)
            sscanf(argv[i++], "%u", &seed);
        printf("seed = %u, n = %d, call = generate_random_segs\n", seed, n);

        srand(seed);
        generate_random_segs(segs, n);

        break;
    case 1:
        if (i < argc)
            sscanf(argv[i++], "%d", &k);
        if (i < argc)
            sscanf(argv[i++], "%lf", &width);
        if (i < argc)
            sscanf(argv[i++], "%u", &seed);
        printf("seed = %u, n = %d, k = %d, width = %lf, call = generate_random_segs_with_k\n", seed, n, k, width);

        srand(seed);
        generate_random_segs_with_k(segs, n, k, width);

        break;
    case 2:
        if (i < argc)
            sscanf(argv[i++], "%lf", &r);
        if (i < argc)
            sscanf(argv[i++], "%u", &seed);
        printf("seed = %u, n = %d, r = %lf, call = generate_random_segs_in_square\n", seed, n, r);

        srand(seed);
        generate_random_segs_in_square(segs, n, r);

        break;
    default:
        printf("seed = %u, n = %d, call = generate_random_segs\n", seed, n);

        srand(seed);
        generate_random_segs(segs, n);
        break;
    }
    FILE *f = fopen("segments.bin", "wb");
    fwrite(&n, sizeof(n), 1, f);
    fwrite(segs, sizeof(*segs), n, f);

    return 0;
}