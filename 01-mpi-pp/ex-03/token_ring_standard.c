#include <stdio.h>
#include <mpi/mpi.h>

#define ITERATIONS 1000
#define MESSAGE_SIZE 1
#define MASTER_RANK 0

int main(int argc, char* argv[])
{
    int rank, numprocs;
    char message = 'a';
    double t_start = 0.0;
    double t_stop = 0.0;
    double sum = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Barrier(MPI_COMM_WORLD);
    
    for(int i = 0; i < ITERATIONS; i++)
    {
        if(rank != MASTER_RANK)
            MPI_Recv(&message, MESSAGE_SIZE, MPI_CHAR, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        else
            t_start = MPI_Wtime();

        MPI_Send(&message, MESSAGE_SIZE, MPI_CHAR, (rank + 1) % numprocs, 0, MPI_COMM_WORLD);

        if(rank == MASTER_RANK)
        {
            MPI_Recv(&message, MESSAGE_SIZE, MPI_CHAR, numprocs - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            t_stop = MPI_Wtime();
            sum += t_stop - t_start;
        }
    }

    if(rank == MASTER_RANK)
    {
        sum /= ITERATIONS;
        printf("Measured time: %f ms\n", sum * 1000);
    }

    fflush(stdout);
    MPI_Finalize();

    return 0;
}
