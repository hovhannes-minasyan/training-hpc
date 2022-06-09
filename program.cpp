#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <string.h>

#include "program.h"

using namespace std;

void run(int rank, int size)
{
    #pragma omp parallel 
    {
        int numThreads = omp_get_num_threads();
        int thId = omp_get_num_threads();
    }
}