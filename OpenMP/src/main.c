#include <stdio.h>
#include <stdlib.h>
#include <time.h>	
#include <unistd.h>
#include <stdint.h>	
#include <omp.h>

#define BILLION 1000000000L
#define LOOP 10000000L

int main(int argc, char *argv[])
{
	struct timespec start, end;
	uint64_t diff=0, barrirer_time=0, no_barrirer_time=0;
    uint64_t max_iterations = LOOP;

    int nthreads;

    if (argc > 1 ) {
        max_iterations= (uint64_t) atoi(argv[1]);
    } // endif //

    printf("Running %ld iterations \n",max_iterations);


    #pragma omp parallel
    {   
        #pragma omp single nowait
        nthreads=omp_get_num_threads();
    } // end of parallel region //

    #pragma omp parallel  private(start, end, diff ) reduction(+:barrirer_time) reduction(+:no_barrirer_time)
    {   
        //int mySelf = omp_get_thread_num();
        for (uint64_t n=0 ; n<max_iterations; ++n) {
        
            #pragma omp barrier
            clock_gettime(CLOCK_MONOTONIC, &start);
            #pragma omp barrier
            clock_gettime(CLOCK_MONOTONIC, &end);
            
            diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
            barrirer_time+=diff;
            
            ////////////////////////////////
        
            #pragma omp barrier
            clock_gettime(CLOCK_MONOTONIC, &start);
            clock_gettime(CLOCK_MONOTONIC, &end);
            
            diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
            no_barrirer_time+=diff;
            
        }	// end for//
        
        //printf("I am %d out of %d,  my barrirer_time: %lu, my no_barrirer_time: %lu\n",mySelf, nthreads,barrirer_time,no_barrirer_time);
    } // end of parallel region //
    
    barrirer_time = barrirer_time / nthreads;
    no_barrirer_time = no_barrirer_time / nthreads;
    //printf("mean my barrirer_time: %lu, mean my no_barrirer_time: %lu\n", barrirer_time, no_barrirer_time);
    
    

    printf("Number of threads: %d\n", nthreads);
    printf ("%lu is the Barrirer estimate; %lu is total time with barrirer; and %lu is the total time with no_barrirer [ nano-seconds] \n",
              (barrirer_time-no_barrirer_time)/(max_iterations),
              barrirer_time/(max_iterations),
              no_barrirer_time/(max_iterations)
    );

    return 0;
} // end main() //

