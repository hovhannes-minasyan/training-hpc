#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#include <mutex>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <thread>

#include "main.h"

const char *lockPath = "/home/common/.lock";

using namespace std;

int main(int argc, char** argv)
{
    MPI_Init(&argc , &argv);
    auto comm = MPI_COMM_WORLD;

    int rank, size;
    MPI_Comm_size(comm , &size);
    MPI_Comm_rank(comm , &rank);
    
    lockProc(rank);

    auto start = MPI_Wtime();
    run(rank, size);
    auto end =  MPI_Wtime();
    
    unlockProc(rank);

    if(rank == 0)
        cout << "Time took to execute " << end - start << "s" << endl;

    MPI_Finalize();

    cout << "Rank " << rank << " exits" << endl;
    return 0;
}

void lockProc(int rank)
{
    int value;
    while(true)
    {
        if(rank == 0)
        value = open(lockPath, O_CREAT|O_EXCL);
    
        MPI_Bcast( &value , 1 , MPI_INT , 0 , MPI_COMM_WORLD);
        if(value > 0)
            break;

        this_thread::sleep_for(3000ms);
    }
}

void unlockProc(int rank)
{
    MPI_Barrier( MPI_COMM_WORLD);
    if(rank == 0)
        remove(lockPath);
}

