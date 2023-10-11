#include <time.h>


#define BILLION 1000000000.0


#define SEC_TO_MCSEC 1000000.0
#define NSEC_TO_MCSEC 1000.0

#define __INIT_DELTA_TIME__ 					\
	double time_d;								\
	struct timespec start, end;					

#define __START_DELTA_TIME__					\
	time_d = 0.0;								\
	timespec_get(&start, TIME_UTC);		

#define __CALC_DELTA_TIME__						\
	timespec_get(&end, TIME_UTC);		\
	time_d = ((end).tv_sec - (start).tv_sec) + 	\
		((end).tv_nsec - (start).tv_nsec) 		\
			/ BILLION

#define __CALC_MC_DELTA_TIME__					\
	timespec_get(&end, TIME_UTC);				\
	time_d = ((end).tv_sec - (start).tv_sec)	\
			* SEC_TO_MCSEC + 					\
		((end).tv_nsec - (start).tv_nsec) 		\
			/ NSEC_TO_MCSEC

#define DELTA_TIME_FORMAT ">> %s: %lf seconds --\n"

#define __print_delta_time(stream)              \
    fprintf((stream), DELTA_TIME_FORMAT, __func__, time_d);	

#define TEST_DELTA_TIME_FORMAT "%lf "

#define __print_test_delta_time(stream)              \
    fprintf((stream), TEST_DELTA_TIME_FORMAT, time_d);




#define __INIT_TIMESPEC__						\
	struct timespec start, end;

#define __START_MARK__(mark) \
	*mark = .0; \
	timespec_get(&start, TIME_UTC);

#define __CALC_MARK_DTIME__(mark) \
	timespec_get(&end, TIME_UTC); \
	*(mark) = ((end).tv_sec - (start).tv_sec)	\
			* SEC_TO_MCSEC + 					\
		((end).tv_nsec - (start).tv_nsec) 		\
			/ NSEC_TO_MCSEC