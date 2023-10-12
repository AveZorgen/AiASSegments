#ifndef GEN_SEG_METH
#define GEN_SEG_METH

#include <stdlib.h>
#include "seg_t.h"

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

void o_bin_dump(pSeg segs, int n) {
    FILE *f = fopen("segments.bin", "wb");
    fwrite(&n, sizeof(n), 1, f);
    fwrite(segs, sizeof(*segs), n, f);
    fclose(f);
}

void i_bin_dump(ppSeg segs, int* n) {
    FILE *f = fopen("segments.bin", "rb");
    fread(n, sizeof(*n), 1, f);
    *segs = malloc(*n * sizeof(Seg));
    fread(*segs, sizeof(Seg), *n, f);
    fclose(f);
}

void __o_stream_dump(pSeg segs, int n, FILE* stream) {
    fprintf(stream, "%d ", n);
    for (int i = 0; i < n; i++) {
        fprintf(stream, "%lf %lf %lf %lf ", segs[i].u.x, segs[i].u.y, segs[i].v.x, segs[i].v.y);
    }
}

void __i_stream_dump(ppSeg segs, int* n, FILE* stream) {
    fscanf(stream, "%d", n);
    *segs = malloc(*n * sizeof(Seg));
    for (int i = 0; i < *n; i++) {
        fscanf(stream, "%lf %lf %lf %lf", &(*segs)[i].u.x, &(*segs)[i].u.y, &(*segs)[i].v.x, &(*segs)[i].v.y);
        (*segs)[i].u.seg_idx = (*segs)[i].v.seg_idx = i;
        (*segs)[i].u.is_left = true;
        (*segs)[i].v.is_left = false;
    } 
}

void o_txt_dump(pSeg segs, int n) {
    FILE *f = fopen("segments.txt", "w");
    __o_stream_dump(segs, n, f);
    fclose(f);
}

void i_txt_dump(ppSeg segs, int* n) {
    FILE *f = fopen("segments.txt", "r");
    __i_stream_dump(segs, n, f);
    fclose(f);
}

void o_kb_dump(pSeg segs, int n) {
    __o_stream_dump(segs, n, stdout);
}

void i_kb_dump(ppSeg segs, int* n) {
    __i_stream_dump(segs, n, stdin);
}

#endif GEN_SEG_METH