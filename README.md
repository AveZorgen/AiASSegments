# AiASSegments
Finding a pair of intersecting segments: naive algorithm and (2,3)-tree based
## User's Guide

## Manual usage

If the $CMake$ project is built with the `-DLAB_TESTS=OFF` flag. The segments are read
from `stdin`, results of algorithms in `stdout`. It is also possible to
to generate segments using the `gen_segs` program.
Prototype: `gen_segs.exe mode n [k r width seed]`
`mode` - method of segment assignment, `n` - number of segments, `seed` - base
random\
`mode=0` - automatic with pseudorandom coordinates of the ends from the
of a unit-length square.\
`mode=1` - automatic with pseudorandom coordinates of the ends from the
a square of length `width`, among which only `k` and `k+1` intersect.\
`mode=2` - automatic with specified length `r` of all segments and
pseudorandom coordinates of their centers from a square of unit length and
pseudorandom values of the angles formed by the lines passing through the
with the abscissa axis. \
Example:\
`stdin`: `gen_segs.exe 0 10 42` \| `main.exe`\
`stdout`:\
${[(0.005341 0.012207) (0.545335 0.917264)], [(0.411115 0.216651) (0.071291 0.708792)]}$\
${[(0.005341 0.012207) (0.545335 0.917264)], [(0.071291 0.708792) (0.411115 0.216651)]}$

## Experimental usage

When the program starts, 5 tests are run, and the results are written to the
corresponding files.
Prototype: `main.exe [seed]`\
Example:\
`stdin`: `main.exe 42`\
`stdout`:\
${>> main: 0.204906 seconds --}$$\
${>> main: 3.130486 seconds --}$$\
${>> main: 4.707294 seconds--}$$\
etc\
output files: `test1_mcs.txt test2_mcs.txt test3_mcs.txt`, etc.

## Programmer's Guide

## Solution Structure

`spi.h` - prototype (2,3)-tree, `spi.c` - its implementation\
`int_t.h, float_t.h, seg_t.h` - types that can be used
as keys in the (2,3)-tree, and their specialized methods\
`spi_test.c` - runnable project for manual testing of the (2,3)-tree\
`timer.h` - time counting macros\
`io_segs.h` - input/output and segment generation functions\
`gen_segs.c` - project to be launched for segment generation\
`main.c` - runnable project for experiments
