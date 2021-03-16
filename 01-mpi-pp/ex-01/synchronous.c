#include <stdio.h>
#include <mpi/mpi.h>

#define MESSAGE_SIZE 20
#define MASTER_RANK 0
#define WORKER_RANK 1

int main(int argc, char* argv[])
{
    int rank, numprocs;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char message[MESSAGE_SIZE];
    if(rank == MASTER_RANK) {
        sprintf(message, "hello");
        MPI_Ssend(message, MESSAGE_SIZE, MPI_CHAR, WORKER_RANK, 0, MPI_COMM_WORLD);
        MPI_Recv(message, MESSAGE_SIZE, MPI_CHAR, WORKER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank 0 received message: %s\n", message);
    } else if(rank == WORKER_RANK) {
        MPI_Recv(message, MESSAGE_SIZE, MPI_CHAR, MASTER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank 1 received message: %s\n", message);
        sprintf(message, "world");
        MPI_Ssend(message, MESSAGE_SIZE, MPI_CHAR, MASTER_RANK, 0, MPI_COMM_WORLD);
    }
    fflush(stdout);
    MPI_Finalize();

    return 0;
}