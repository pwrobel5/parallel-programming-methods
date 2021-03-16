#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>

#define ITERATIONS 5000
#define MESSAGE_SIZE 1
#define MASTER_RANK 0
#define WORKER_RANK 1

int main(int argc, char* argv[])
{
    int rank;
    char message = 'a';
    double t_start = 0.0;
    double t_stop = 0.0;
    double sum = 0.0;
    int packed_size, buffer_size;
    char* buffer;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Pack_size(MESSAGE_SIZE, MPI_CHAR, MPI_COMM_WORLD, &packed_size);
    buffer_size = 3 * MPI_BSEND_OVERHEAD + packed_size;
    buffer = malloc(buffer_size * sizeof(char));
    if(buffer == NULL)
    {
        MPI_Finalize();
        perror("Error with memory allocation");
        exit(EXIT_FAILURE);
    }

    MPI_Buffer_attach(buffer, buffer_size);

    for(int i = 0; i < ITERATIONS; i++)
    {
        if(rank == MASTER_RANK)
        {
            t_start = MPI_Wtime();
            MPI_Bsend(&message, MESSAGE_SIZE, MPI_CHAR, WORKER_RANK, 0, MPI_COMM_WORLD);
            MPI_Recv(&message, MESSAGE_SIZE, MPI_CHAR, WORKER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            t_stop = MPI_Wtime();
            sum += t_stop - t_start;
        }
        else
        {
            MPI_Recv(&message, MESSAGE_SIZE, MPI_CHAR, MASTER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Bsend(&message, MESSAGE_SIZE, MPI_CHAR, MASTER_RANK, 0, MPI_COMM_WORLD);
        }
    }

    if(rank == MASTER_RANK)
    {
        sum /= ITERATIONS;
        printf("Measured delay: %f ms\n", sum * 1000);
    }

    fflush(stdout);
    MPI_Buffer_detach(buffer, &buffer_size);
    free(buffer);
    MPI_Finalize();

    return 0;
}