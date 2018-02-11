#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

#define NANO 1.0e-9
#define LOOP 1.0e8


int main(int argc, char *argv[])
{
    struct timeval tp;
    double elapsed_time, barrirer_time=0.0, no_barrirer_time=0.0;
    unsigned int max_iterations = (unsigned int) LOOP;

    const int nthreads=1;

    if (argc > 1 ) {
        max_iterations= (unsigned int ) atoi(argv[1]);
    } // endif //

    printf("Running %d iterations \n",max_iterations);



    for (unsigned int n=1 ; n<=max_iterations; ++n) {
        #pragma omp parallel num_threads(nthreads)
        {
            gettimeofday(&tp,NULL);
            elapsed_time = -(tp.tv_sec + tp.tv_usec/1.0e6);
            #pragma omp barrier
            gettimeofday(&tp,NULL);
            elapsed_time += (tp.tv_sec + tp.tv_usec/1.0e6);
            barrirer_time+=elapsed_time;

            gettimeofday(&tp,NULL);
            elapsed_time = -(tp.tv_sec + tp.tv_usec/1.0e6);
            // no barrier here
            gettimeofday(&tp,NULL);
            elapsed_time += (tp.tv_sec + tp.tv_usec/1.0e6);
            no_barrirer_time+=elapsed_time;
        } // end of parallel region //

    }	// end of time loop n = 1,...,nstep //

    //printf ("\n\nIt tooks %14.6e seconds for %d threads to finish\n", barrirer_time, nthreads);
    printf ("\n\nIt tooks %14.6g nanao-seconds with barrirer \n", barrirer_time/(LOOP*NANO) );
    printf ("It tooks %14.6g nanao-seconds with no_barrirer\n", no_barrirer_time/(LOOP*NANO));
    printf ("It tooks %14.6g nanao-seconds barrirer estimate\n", (barrirer_time-no_barrirer_time)/(LOOP*NANO));

    return 0;
} // end main() //

