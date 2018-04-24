#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <mpi.h>

#define BILLION 1000000000L
#define LOOP 1000000L

int main(int argc, char *argv[])
{
	struct timespec start, end;
	uint64_t diff, barrier_time=0, no_barrier_time=0;
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
        MPI_Allreduce( MPI_IN_PLACE, &diff, 1, MPI_UNSIGNED_LONG, MPI_MIN,MPI_COMM_WORLD );
        barrier_time+=diff;


        MPI_Barrier(MPI_COMM_WORLD);
        clock_gettime(CLOCK_MONOTONIC, &start);
        // no barrier here
        clock_gettime(CLOCK_MONOTONIC, &end);
        diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        MPI_Allreduce( MPI_IN_PLACE, &diff, 1, MPI_UNSIGNED_LONG, MPI_MIN,MPI_COMM_WORLD );
        no_barrier_time+=diff;

    }	// end for //


    if (rank == root) {
        printf ("%lu is the Barrier estimate; %lu is total time with barrier; and %lu is the total time with no_barrier [ nano-seconds]",
                  (barrier_time-no_barrier_time)/(max_iterations),
                  barrier_time/(max_iterations),
                  no_barrier_time/(max_iterations)
        );
        printf(", number of processes: %d\n", size);
    } // end if

    MPI_Finalize();

    return 0;
} // end main() //

