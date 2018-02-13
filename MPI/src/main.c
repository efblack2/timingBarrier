#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>

#define NANO 1.0e-9
#define LOOP 1.0e8

int main(int argc, char *argv[])
{
    struct timeval tp;
    double elapsed_time, barrirer_time=0.0, no_barrirer_time=0.0;
    unsigned int max_iterations = (unsigned int) LOOP;

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
    } // end if //

    if (rank == root) {
        printf("Running %d iterations \n",max_iterations);fflush(stdout);
    } // end if




    for (unsigned int n=1 ; n<=max_iterations; ++n) {

        gettimeofday(&tp,NULL);
        elapsed_time = -(tp.tv_sec + tp.tv_usec/1.0e6);
        MPI_Barrier(MPI_COMM_WORLD);
        gettimeofday(&tp,NULL);
        elapsed_time += (tp.tv_sec + tp.tv_usec/1.0e6);
        barrirer_time+=elapsed_time;

        gettimeofday(&tp,NULL);
        elapsed_time = -(tp.tv_sec + tp.tv_usec/1.0e6);
        // no marrirer here
        gettimeofday(&tp,NULL);
        elapsed_time += (tp.tv_sec + tp.tv_usec/1.0e6);
        no_barrirer_time+=elapsed_time;

/*
        elapsed_time = -MPI_Wtime();
        MPI_Barrier(sm_comm);
        elapsed_time += MPI_Wtime();
        barrirer_time+=elapsed_time;

        elapsed_time = -MPI_Wtime();
        elapsed_time += MPI_Wtime();
        no_barrirer_time+=elapsed_time;
*/


    }	// end of time loop n = 1,...,nstep //

    if (rank == root) {
        printf ("%10.6g is the Barrirer estimate; %10.6g is total time with barrirer; and %10.6g is the total time with no_barrirer [ nano-seconds] \n",
                  (barrirer_time-no_barrirer_time)/(max_iterations*NANO),
                  barrirer_time/(max_iterations*NANO),
                  no_barrirer_time/(max_iterations*NANO)
        );
    } // end if

    MPI_Finalize();

    return 0;
} // end main() //

