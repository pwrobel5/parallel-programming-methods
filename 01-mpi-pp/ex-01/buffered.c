#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>

#define MESSAGE_SIZE 20
#define MASTER_RANK 0
#define WORKER_RANK 1

int main(int argc, char* argv[])
{
    int rank, numprocs;
    int packed_size, buffer_size;
    char* buffer;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Pack_size(MESSAGE_SIZE, MPI_CHAR, MPI_COMM_WORLD, &packed_size);
    buffer_size = 3 * MPI_BSEND_OVERHEAD + packed_size;
    buffer = malloc(buffer_size * sizeof(char));
    MPI_Buffer_attach(buffer, buffer_size);

    char message[MESSAGE_SIZE];
    if(rank == MASTER_RANK) {
        sprintf(message, "hello");
        MPI_Bsend(message, MESSAGE_SIZE, MPI_CHAR, WORKER_RANK, 0, MPI_COMM_WORLD);
        MPI_Recv(message, MESSAGE_SIZE, MPI_CHAR, WORKER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank 0 received message: %s\n", message);
    } else if(rank == WORKER_RANK) {
        MPI_Recv(message, MESSAGE_SIZE, MPI_CHAR, MASTER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank 1 received message: %s\n", message);
        sprintf(message, "world");
        MPI_Bsend(message, MESSAGE_SIZE, MPI_CHAR, MASTER_RANK, 0, MPI_COMM_WORLD);
    }
    fflush(stdout);
    MPI_Buffer_detach(buffer, &buffer_size);
    free(buffer);
    MPI_Finalize();

    return 0;
}