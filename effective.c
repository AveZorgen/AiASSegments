#include <stdlib.h>

#include "spi.h"
#include "io_segs.h"
#include "timer.h"
// AiAsSegments

// #define DEBUG
// #define TESTS
// #define GOOD_AT_K_NAIVE

#ifndef TESTS
#define DEBUG
#endif


// sort methods
void pnt_swap(pPoint a, pPoint b) {
    Point r = *a;
    *a = *b;
    *b = r;
}

int partition(pPoint array, int low, int high) {
    double pivot = array[high].x;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (array[j].x < pivot) {
            i++;
            pnt_swap(&array[i], &array[j]);
        }
    }

    pnt_swap(&array[i + 1], &array[high]);

    return (i + 1);
}

void quickSort(pPoint array, int low, int high) {
    if (low < high) {
        int pi = partition(array, low, high);
        quickSort(array, low, pi - 1);
        quickSort(array, pi + 1, high);
    }
}

void lex_sort(pPoint array, int n) {
    quickSort(array, 0, n - 1);
}
// ---sort methods


bool fill_points(pPoint points, pSeg segs, int segs_len);
bool intersection_effective(pSeg segs, int segs_len, pPoint points, pSeg s1, pSeg s2, double *sort, double *eff_alg);

bool intersection_naive(pSeg segs, int n, pSeg seg1, pSeg seg2, double *naive_alg);
bool intersection_naive_j_to_i(pSeg segs, int n, pSeg seg1, pSeg seg2, double *naive_alg);

void launch_effective(pSeg segs, int n, FILE *const _Stream);
void launch_naive(pSeg segs, int n, FILE *const _Stream);


#ifdef TESTS
// tests
// n t1 t2 t3
void test1() {
    FILE *f = fopen("test1_mcs_good_k.txt", "w");
    pSeg segs;
    segs = malloc(10001 * sizeof(Seg)); // empty segs
    for (int n = 1; n < 10001 + 1; n += 100) {

        generate_random_segs(segs, n); // fill segs
        // o_txt_dump(segs, n);

        fprintf(f, "%i ", n);
        launch_naive(segs, n, f); // segs does not changed
        // launch_effective(segs, n, f);
        fprintf(f, "\n");
    }
    free(segs);
    fclose(f);
}

// k t1 t2 t3
void test2() {
    FILE *f = fopen("test2_mcs_good_k.txt", "w");
    int n = 10003;
    pSeg segs = malloc(n * sizeof(Seg));
    for (int k = 1; k < 10001 + 1; k += 100) {

        generate_random_segs_with_k(segs, n, k, 1.0);
        // o_txt_dump(segs, n);

        fprintf(f, "%i ", k);
        launch_naive(segs, n, f);
        // launch_effective(segs, n, f);
        fprintf(f, "\n");
    }
    free(segs);
    fclose(f);
}

// n t1 t2 t3
void test3() {
    FILE *f = fopen("test3_mcs_good_k.txt", "w");
    double r = 0.001;
    pSeg segs;
    segs = malloc(10001 * sizeof(Seg));
    for (int n = 1; n < 10001 + 1; n += 100) {

        generate_random_segs_in_square(segs, n, r);
        // o_txt_dump(segs, n);

        fprintf(f, "%i ", n);
        launch_naive(segs, n, f);
        // launch_effective(segs, n, f);
        fprintf(f, "\n");
    }
    free(segs);
    fclose(f);
}

// r t1 t2 t3
void test4() {
    FILE *f = fopen("test4_mcs_good_k.txt", "w");
    int n = 10000;
    pSeg segs = malloc(n * sizeof(Seg));
    for (double r = 0.0001; r < 0.01; r += 0.0001) {

        generate_random_segs_in_square(segs, n, r);

        fprintf(f, "%lf ", r);
        launch_naive(segs, n, f);
        // launch_effective(segs, n, f);
        fprintf(f, "\n");
    }
    free(segs);
    fclose(f);
}

// n t1 t2 t3
void test5() {
    FILE *f = fopen("test5_mcs_good_k.txt", "w");
    pSeg segs;
    segs = malloc(15001 * sizeof(Seg));
    for (int n = 1; n < 15001 + 1; n += 100) {
        generate_random_segs_with_k(segs, n, n, 50.0);
        // if (n == 9501) 
        //     o_txt_dump(segs, n);
        fprintf(f, "%i ", n);
        launch_naive(segs, n, f);
        // launch_effective(segs, n, f);
        fprintf(f, "\n");

#ifdef DEBUG
        // printf("\n");
#endif // DEBUG
    }
    // int n = 9501;
    // i_txt_dump(&segs, &n);
    // launch_effective(segs, n, f);
    free(segs);
    fclose(f);
}
#endif // tests



int main(int argc, char **argv) {
#ifdef TESTS
    int seed = 0;
    if (argc == 2) {
        seed = atoi(argv[1]);
    }
    __INIT_DELTA_TIME__
    srand(seed);
    __START_DELTA_TIME__
    test1(); // 0.174560
    __CALC_DELTA_TIME__;
    __print_delta_time(stdout);

    __START_DELTA_TIME__
    test3(); // 2.849994
    __CALC_DELTA_TIME__;
    __print_delta_time(stdout);

    __START_DELTA_TIME__
    test4(); // 2.903829
    __CALC_DELTA_TIME__;
    __print_delta_time(stdout);

    __START_DELTA_TIME__
    test2(); // 1586.071516
    __CALC_DELTA_TIME__;
    __print_delta_time(stdout);

    // srand(503);
    __START_DELTA_TIME__
    test5(); // 2162.369920
    __CALC_DELTA_TIME__;
    __print_delta_time(stdout);

#else
    int n;
    pSeg segs;
    i_kb_dump(&segs, &n);
    // generate_random_segs(segs, n);
    // generate_random_segs_with_k(segs, n, n - 2, 10.0);
    // generate_random_segs_in_square(segs, n, 0.05);
    // o_txt_dump(segs, n);

    Seg seg1, seg2;
    double time1, time2, time3;

    bool try_intersect = intersection_naive(segs, n, &seg1, &seg2, &time1);
    if (try_intersect) {
        printf("between [(%lf %lf) (%lf %lf)], [(%lf %lf) (%lf %lf)]\n", seg1.u.x, seg1.u.y, seg1.v.x, seg1.v.y,
               seg2.u.x, seg2.u.y, seg2.v.x, seg2.v.y);
    }

    pPoint points = (pPoint)malloc(2 * n * sizeof(Point));
    fill_points(points, segs, n);
    try_intersect = intersection_effective(segs, n, points, &seg1, &seg2, &time2, &time3);
    if (try_intersect) {
        printf("between [(%lf %lf) (%lf %lf)], [(%lf %lf) (%lf %lf)]\n", seg1.u.x, seg1.u.y, seg1.v.x, seg1.v.y,
               seg2.u.x, seg2.u.y, seg2.v.x, seg2.v.y);
    }
    free(points);
    free(segs);
#endif

    return 0;
}


bool fill_points(pPoint points, pSeg segs, int segs_len) {
    Point tmp;
    for (int i = 0; i < segs_len; i++) {
        if (segs[i].u.x >= segs[i].v.x) {
            if (segs[i].u.x != segs[i].v.x) {
                pnt_swap(&segs[i].u, &segs[i].v);
                segs[i].u.is_left = true;
                segs[i].v.is_left = false;
            } else if (segs[i].u.y > segs[i].v.y) {
                pnt_swap(&segs[i].u, &segs[i].v);
                segs[i].u.is_left = true;
                segs[i].v.is_left = false;
            } else if (segs[i].u.y == segs[i].v.y) {
                return true;
            }
        }
        points[2 * i] = segs[i].u;
        points[2 * i + 1] = segs[i].v;
    }
    return false;
}

bool intersection_effective(pSeg segs, int segs_len, pPoint points, pSeg s1, pSeg s2, double *sort, double *eff_alg) {
    __INIT_TIMESPEC__

    __START_MARK__(sort);
    lex_sort(points, 2 * segs_len);
    __CALC_MARK_DTIME__(sort);

    bool res = false;
    pSeg s;
    Point pnt;
    pNode root = NULL;
    double k;

    __START_MARK__(eff_alg);
    for (int i = 0; i < 2 * segs_len - 1; i++) {
        pnt = points[i];
        s = &segs[pnt.seg_idx];
        s->t_as_param = pnt.x;

        if (pnt.is_left) {
            seg_set_k(s);

            root = insert(root, s);

            pSeg suc_seg = successor_seg(root, s);
            if (suc_seg && suc_seg != s) {
                if (intersect(suc_seg, s)) {
#ifdef DEBUG
                    *s1 = *s;
                    *s2 = *suc_seg;
#endif // DEBUG
                    res = true;
                    break;
                }
            }
            pSeg pred_seg = predecessor_seg(root, s);
            if (pred_seg && pred_seg != s) {
                if (intersect(pred_seg, s)) {
#ifdef DEBUG
                    *s1 = *pred_seg;
                    *s2 = *s;
#endif // DEBUG
                    res = true;
                    break;
                }
            }
        } else {
            pSeg suc_seg = successor_seg(root, s);
            pSeg pred_seg = predecessor_seg(root, s);
            if (suc_seg && pred_seg && suc_seg != pred_seg) {
                if (intersect(suc_seg, pred_seg)) {
#ifdef DEBUG
                    *s1 = *pred_seg;
                    *s2 = *suc_seg;
#endif // DEBUG
                    res = true;
                    break;
                }
            }
            root = del(root, s);
        }
    }
    __CALC_MARK_DTIME__(eff_alg);
    return res;
}

bool intersection_naive(pSeg segs, int n, pSeg seg1, pSeg seg2, double *naive_alg) {
    bool res = false;
    __INIT_TIMESPEC__
    __START_MARK__(naive_alg)
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (intersect(&segs[i], &segs[j])) {
#ifdef DEBUG
                *seg1 = segs[i];
                *seg2 = segs[j];
#endif // DEBUG
                res = true;
                break;
            }
        }
        if (res)
            break;
    }
    __CALC_MARK_DTIME__(naive_alg);
    return res;
}

bool intersection_naive_j_to_i(pSeg segs, int n, pSeg seg1, pSeg seg2, double *naive_alg) {
    bool res = false;
    __INIT_TIMESPEC__
    __START_MARK__(naive_alg)
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (intersect(&segs[i], &segs[j])) {
#ifdef DEBUG
                *seg1 = segs[i];
                *seg2 = segs[j];
#endif // DEBUG
                res = true;
                break;
            }
        }
        if (res)
            break;
    }
    __CALC_MARK_DTIME__(naive_alg);
    return res;
}

void launch_effective(pSeg segs, int n, FILE *const _Stream) {
    Seg seg1, seg2;
    pPoint points = (pPoint)malloc(2 * n * sizeof(Point)); // empty points
    double sort_res = -1.0, alg_res = -1.0;
    if (fill_points(points, segs, n)) { // sort internal points in segs, fills points
        fprintf(_Stream, "%lf %lf ", .0, .0);
        return;
    }
    intersection_effective(segs, n, points, &seg1, &seg2, &sort_res, &alg_res); // sort points

#ifdef DEBUG
    printf("%i: [(%lf %lf) (%lf %lf)], %i: [(%lf %lf) (%lf %lf)]\n", seg1.u.seg_idx, seg1.u.x, seg1.u.y, seg1.v.x, seg1.v.y, seg2.u.seg_idx, seg2.u.x, seg2.u.y, seg2.v.x, seg2.v.y);
    if (seg1.u.seg_idx == seg2.u.seg_idx && seg1.u.seg_idx != -1) {
        printf("With itself\n");
    }
    if (!intersect(&seg1, &seg2) && seg1.u.seg_idx != -1) {
        printf("Not intersect!");
    }
#endif // DEBUG
    fprintf(_Stream, "%lf %lf ", sort_res, alg_res);

    free(points);
}

void launch_naive(pSeg segs, int n, FILE *const _Stream) {
    Seg seg1, seg2;
    double alg_res = -1.0;

#ifdef GOOD_AT_K_NAIVE
    intersection_naive_j_to_i(segs, n, &seg1, &seg2, &alg_res);
#else
    intersection_naive(segs, n, &seg1, &seg2, &alg_res);
#endif

#ifdef DEBUG
    printf("%i: [(%lf %lf) (%lf %lf)], %i: [(%lf %lf) (%lf %lf)]\n", seg1.u.seg_idx, seg1.u.x, seg1.u.y, seg1.v.x, seg1.v.y, seg2.u.seg_idx, seg2.u.x, seg2.u.y, seg2.v.x, seg2.v.y);
#endif // DEBUG
    fprintf(_Stream, "%lf ", alg_res);
}