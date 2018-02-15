#include <stdio.h>
#include <stdlib.h>
#include <time.h>	
#include <unistd.h>
#include <stdint.h>	
#include <omp.h>

#define BILLION 1000000000L
#define LOOP 100000000L


int main(int argc, char *argv[])
{
	struct timespec start, end;
	uint64_t diff, barrirer_time=0, no_barrirer_time=0;
    uint64_t max_iterations = LOOP;

    const int nthreads=1;

    if (argc > 1 ) {
        max_iterations= (uint64_t) atoi(argv[1]);
    } // endif //

    printf("Running %ld iterations \n",max_iterations);
    


    for (uint64_t n=1 ; n<=max_iterations; ++n) {
        #pragma omp parallel num_threads(nthreads)
        {
            clock_gettime(CLOCK_MONOTONIC, &start);
            #pragma omp barrier
            clock_gettime(CLOCK_MONOTONIC, &end);
            diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
            barrirer_time+=diff;

            clock_gettime(CLOCK_MONOTONIC, &start);
            // no barrier here
            clock_gettime(CLOCK_MONOTONIC, &end);
            diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
            no_barrirer_time+=diff;
            
        } // end of parallel region //
    }	// end of time loop n = 1,...,nstep //

    printf ("%lu is the Barrirer estimate; %lu is total time with barrirer; and %lu is the total time with no_barrirer [ nano-seconds] \n",
              (barrirer_time-no_barrirer_time)/(max_iterations),
              barrirer_time/(max_iterations),
              no_barrirer_time/(max_iterations)
    );

    return 0;
} // end main() //

