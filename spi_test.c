#include "spi.h"
#include "timer.h"



int main(void) {
    #ifdef INT_TEST

    __INIT_DELTA_TIME__;
    __START_DELTA_TIME__;

    pNode root = NULL;
    for (int i = 0; i < 15; ++i) {
        root = insert(root, (i + 1)*10);
    }
    root = insert(root, 5);
    root = insert(root, 15);
    root = insert(root, 25);
    root = insert(root, 8);
    // print(root, 0);
    root = del(root, 5);
    root = del(root, 8);
    root = del(root, 10);
    root = del(root, 30);
    root = del(root, 15);

    __CALC_DELTA_TIME__;
    __print_delta_time(stdout);

    print(root, 0);
    print(suc(root, 50), 0); //60
    print(suc(root, 70), 0); //80
    print(suc(root, 60), 0); //70
    print(pred(root, 60), 0); //50
    print(pred(root, 80), 0); //70
    print(pred(root, 70), 0); //60

    #endif // INT_TEST

    #ifdef FLOAT_TEST

    pNode root = NULL;
    for (int i = 0; i < 7; ++i) {
        root = insert(root, i*0.5 + 1);
    }
    print(root, 0);

    #endif // FLOAT_TEST

    #ifdef SEG_TEST

    pNode root = NULL;

    Seg seg0 =  { .u={0, 0, 0, true }, .v={6, 6, 0, false} };
    seg_set_k(&seg0);

    seg0.t_as_param = seg0.u.x;
    root = insert(root, &seg0);
    print(root, 0);


    Seg seg1 = { .u={3, 1, 1, true }, .v={7, 5, 1, false } };
    seg_set_k(&seg1);

    seg1.t_as_param = seg1.u.x;
    root = insert(root, &seg1);
    print(root, 0);


    Seg seg2 = { .u={4, 3, 2, true }, .v={5, 7, 2, false } };
    seg_set_k(&seg2);

    seg2.t_as_param = seg2.u.x;
    root = insert(root, &seg2);
    print(root, 0);



    seg2.t_as_param = seg2.v.x;
    root = del(root, &seg2);
    print(root, 0);

    seg0.t_as_param = seg0.v.x;
    root = del(root, &seg0);
    print(root, 0);

    seg1.t_as_param = seg1.v.x;
    root = del(root, &seg1);
    print(root, 0);

    #endif // SEG_TEST

    return 0;
}