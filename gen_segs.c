#include <stdio.h>
#include <string.h>

#include "seg_t.h"
#include "io_segs.h"


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
        // printf("seed = %u, n = %d, call = generate_random_segs\n", seed, n);

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
        // printf("seed = %u, n = %d, k = %d, width = %lf, call = generate_random_segs_with_k\n", seed, n, k, width);

        srand(seed);
        generate_random_segs_with_k(segs, n, k, width);

        break;
    case 2:
        if (i < argc)
            sscanf(argv[i++], "%lf", &r);
        if (i < argc)
            sscanf(argv[i++], "%u", &seed);
        // printf("seed = %u, n = %d, r = %lf, call = generate_random_segs_in_square\n", seed, n, r);

        srand(seed);
        generate_random_segs_in_square(segs, n, r);

        break;
    default:
        // printf("seed = %u, n = %d, call = generate_random_segs\n", seed, n);

        srand(seed);
        generate_random_segs(segs, n);
        break;
    }

    o_kb_dump(segs, n);
    printf("\n");
/*
    o_bin_dump(segs, n);

    memset(segs, 0, n * sizeof(*segs));

    free(segs);
    i_bin_dump(segs, &n);

    o_kb_dump(segs, n);
    printf("\n");
*/
    free(segs);
    return 0;
}