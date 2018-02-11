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

    MPI_Comm sm_comm;
    MPI_Comm_split_type(MPI_COMM_WORLD,MPI_COMM_TYPE_SHARED, 0,MPI_INFO_NULL, &sm_comm);

    int mySharedRank,sharedSize;
    MPI_Comm_rank(sm_comm,&mySharedRank);
    MPI_Comm_size(sm_comm,&sharedSize);

    if (sharedSize>1) {
        if (mySharedRank == root) printf("Please, run using only one processor.\nBye....\n");
        MPI_Finalize();
        exit(0);
    } // end if //


    if (argc > 1 ) {
        max_iterations=atoi(argv[1]);
    } // end if //

    if (mySharedRank == root) {
        printf("Running %d iterations \n",max_iterations);fflush(stdout);
    } // end if




    for (unsigned int n=1 ; n<=max_iterations; ++n) {

        gettimeofday(&tp,NULL);
        elapsed_time = -(tp.tv_sec + tp.tv_usec/1.0e6);
        MPI_Barrier(sm_comm);
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

    if (mySharedRank == root) {
        printf ("\n\nIt tooks %14.6g nanao-seconds with barrirer \n", barrirer_time/(LOOP*NANO) );
        printf ("It tooks %14.6g nanao-seconds with no_barrirer\n", no_barrirer_time/(LOOP*NANO));
        printf ("It tooks %14.6g nanao-seconds barrirer estimate\n", (barrirer_time-no_barrirer_time)/(LOOP*NANO));
    } // end if

    MPI_Finalize();

    return 0;
} // end main() //

