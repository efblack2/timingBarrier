#include <stdio.h>
#include <stdlib.h>
#include <time.h>	
#include <unistd.h>
#include <stdint.h>	
#include <mpi.h>

#define BILLION 1000000000L
#define LOOP 10000000L


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


    if (argc > 1 ) {
        max_iterations= (uint64_t) atoi(argv[1]);
    } // end if //

    if (rank == root) {
        printf("Running %ld iterations \n",max_iterations);fflush(stdout);
    } // end if


    for (uint64_t n=0 ; n<max_iterations; ++n) {
    
        MPI_Barrier(MPI_COMM_WORLD);
        clock_gettime(CLOCK_MONOTONIC, &start);
        MPI_Barrier(MPI_COMM_WORLD);
        clock_gettime(CLOCK_MONOTONIC, &end);
        diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        barrirer_time+=diff;



        MPI_Barrier(MPI_COMM_WORLD);
        clock_gettime(CLOCK_MONOTONIC, &start);
        // no barrirer here
        clock_gettime(CLOCK_MONOTONIC, &end);
        diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        no_barrirer_time+=diff;
        
    }	// end for //
    
    //printf("I am %d out of %d,  my barrirer_time: %lu, my no_barrirer_time: %lu\n",rank, size,barrirer_time,no_barrirer_time);
    
    MPI_Allreduce( MPI_IN_PLACE, &barrirer_time,    1, MPI_UNSIGNED_LONG, MPI_SUM,MPI_COMM_WORLD );
    MPI_Allreduce( MPI_IN_PLACE, &no_barrirer_time, 1, MPI_UNSIGNED_LONG, MPI_SUM,MPI_COMM_WORLD );


    
    if (rank == root) {
        barrirer_time = barrirer_time / size;
        no_barrirer_time = no_barrirer_time / size;
        //printf("mean my barrirer_time: %lu, mean my no_barrirer_time: %lu\n", barrirer_time, no_barrirer_time);

        printf("Number of processors: %d\n", size);
        printf ("%lu is the Barrirer estimate; %lu is total time with barrirer; and %lu is the total time with no_barrirer [ nano-seconds] \n",
                  (barrirer_time-no_barrirer_time)/(max_iterations),
                  barrirer_time/(max_iterations),
                  no_barrirer_time/(max_iterations)
        );
    } // end if

    MPI_Finalize();

    return 0;
} // end main() //

