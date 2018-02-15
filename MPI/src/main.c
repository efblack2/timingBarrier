#include <stdio.h>
#include <stdlib.h>
#include <time.h>	
#include <unistd.h>
#include <stdint.h>	
#include <mpi.h>

#define BILLION 1000000000L
#define LOOP 100000000L


int main(int argc, char *argv[])
{
	struct timespec start, end;
	uint64_t diff, barrirer_time=0, no_barrirer_time=0;
    uint64_t max_iterations = LOOP;

    const int root=0;
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if (size>1) {
        if (rank == root) printf("Please, run using only one processor.\nBye....\n");
        MPI_Finalize();
        exit(0);
    } // end if //


    if (argc > 1 ) {
        max_iterations=atoi(argv[1]);
        max_iterations= (uint64_t) atoi(argv[1]);
    } // end if //

    if (rank == root) {
        printf("Running %ld iterations \n",max_iterations);fflush(stdout);
    } // end if


    for (uint64_t n=1 ; n<=max_iterations; ++n) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        MPI_Barrier(MPI_COMM_WORLD);
        clock_gettime(CLOCK_MONOTONIC, &end);
        diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        barrirer_time+=diff;

        clock_gettime(CLOCK_MONOTONIC, &start);
        // no barrirer here
        clock_gettime(CLOCK_MONOTONIC, &end);
        diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        no_barrirer_time+=diff;
        
    }	// end of time loop n = 1,...,nstep //

    if (rank == root) {
        printf ("%lu is the Barrirer estimate; %lu is total time with barrirer; and %lu is the total time with no_barrirer [ nano-seconds] \n",
                  (barrirer_time-no_barrirer_time)/(max_iterations),
                  barrirer_time/(max_iterations),
                  no_barrirer_time/(max_iterations)
        );
    } // end if

    MPI_Finalize();

    return 0;
} // end main() //

