#include <stdio.h>
#include <mpi/mpi.h>

#define MESSAGE_SIZE 20
#define MASTER_RANK 0
#define WORKER_RANK 1

int main(int argc, char* argv[])
{
    int rank, numprocs;
    MPI_Request request_send, request_receive;
    MPI_Status status_send, status_receive;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char message_sent[MESSAGE_SIZE];
    char message_received[MESSAGE_SIZE];
    if(rank == MASTER_RANK) {
        sprintf(message_sent, "hello");
        MPI_Isend(message_sent, MESSAGE_SIZE, MPI_CHAR, WORKER_RANK, 0, MPI_COMM_WORLD, &request_send);
        MPI_Wait(&request_send, &status_send);
        
        MPI_Irecv(message_received, MESSAGE_SIZE, MPI_CHAR, WORKER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD, &request_receive);
        MPI_Wait(&request_receive, &status_receive);
        printf("Rank 0 received message: %s\n", message_received);
    } else if(rank == WORKER_RANK) {
        MPI_Irecv(message_received, MESSAGE_SIZE, MPI_CHAR, MASTER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD, &request_receive);
        MPI_Wait(&request_receive, &status_receive);
        printf("Rank 1 received message: %s\n", message_received);

        sprintf(message_sent, "world");
        MPI_Isend(message_sent, MESSAGE_SIZE, MPI_CHAR, MASTER_RANK, 0, MPI_COMM_WORLD, &request_send);
        MPI_Wait(&request_send, &status_send);
    }
    fflush(stdout);
    MPI_Finalize();

    return 0;
}