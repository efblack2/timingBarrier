#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <omp.h>

#define BILLION 1000000000L
#define LOOP 1000000L

int main(int argc, char *argv[])
{
	struct timespec start, end;
	uint64_t barrier_time=0, no_barrier_time=0;
    uint64_t max_iterations = LOOP;
    uint64_t minimum;
    int size;

    if (argc > 1 ) {
        max_iterations= (uint64_t) atoi(argv[1]);
    } // endif //

    printf("Running %ld iterations \n",max_iterations);


    #pragma omp parallel
    {
        #pragma omp master
        size=omp_get_num_threads();
    } // end of parallel region //

    uint64_t *diff = (uint64_t *) malloc(size * sizeof (uint64_t));

    #pragma omp parallel  private(start, end ) //reduction(+:barrier_time) reduction(+:no_barrier_time)
    {
        int rank = omp_get_thread_num();

        for (uint64_t n=0 ; n<max_iterations; ++n) {

            #pragma omp barrier
            clock_gettime(CLOCK_MONOTONIC, &start);
            #pragma omp barrier
            clock_gettime(CLOCK_MONOTONIC, &end);

            diff[rank] = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;

            #pragma omp barrier
            #pragma omp master
            {
                minimum = diff[0];
                for ( int c = 1; c < size ; ++c ) {
                    if ( diff[c] < minimum ) {
                       minimum = diff[c];
                    } // end if //
                }  // end for //
                barrier_time+=minimum;
            } // end of master region//

            ////////////////////////////////

            #pragma omp barrier
            clock_gettime(CLOCK_MONOTONIC, &start);
            // no barrirer here
            clock_gettime(CLOCK_MONOTONIC, &end);

            diff[rank] = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;

            #pragma omp barrier
            #pragma omp master
            {
                minimum = diff[0];
                for ( int c = 1; c < size ; ++c ) {
                    if ( diff[c] < minimum ) {
                       minimum = diff[c];
                    } // end if //
                }  // end for //
                no_barrier_time+=minimum;
            } // end of master region//

        }	// end for//
    } // end of parallel region //

    printf ("%lu is the Barrier estimate; %lu is total time with barrier; and %lu is the total time with no_barrier [ nano-seconds]",
              (barrier_time-no_barrier_time)/(max_iterations),
              barrier_time/(max_iterations),
              no_barrier_time/(max_iterations)
    );
    printf(", number of threads: %d\n", size);

    free(diff);

    return 0;
} // end main() //

