#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>

#define ITERATIONS 5000
#define CORRECT_ARGC 3
#define BYTES_POSITION 1
#define FILE_NAME_POSITION 2
#define MASTER_RANK 0
#define WORKER_RANK 1
#define CAPACITY_FACTOR (8.0 / 1000000)

int main(int argc, char* argv[])
{
    int rank;
    double t_start = 0.0;
    double t_stop = 0.0;
    double sum = 0.0;

    if(argc != CORRECT_ARGC)
    {
        perror("Incorrect number of input arguments");
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    int message_size = atoi(argv[BYTES_POSITION]);
    char* file_name = argv[FILE_NAME_POSITION];
    char message[message_size];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Barrier(MPI_COMM_WORLD);

    for(int i = 0; i < ITERATIONS; i++)
    {
        if(rank == MASTER_RANK)
        {
            t_start = MPI_Wtime();
            MPI_Send(message, message_size, MPI_CHAR, WORKER_RANK, 0, MPI_COMM_WORLD);
            MPI_Recv(message, message_size, MPI_CHAR, WORKER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            t_stop = MPI_Wtime();
            sum += t_stop - t_start;
        }
        else
        {
            MPI_Recv(message, message_size, MPI_CHAR, MASTER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(message, message_size, MPI_CHAR, MASTER_RANK, 0, MPI_COMM_WORLD);
        }
    }

    if(rank == MASTER_RANK)
    {
        printf("\nMessage size: %d\n", message_size);
        sum /= ITERATIONS;
        printf("\tMeasured delay: %f ms\n", sum * 1000); // 1000 for ms

        FILE* output_file = fopen(file_name, "a");
        if(output_file == NULL)
        {
            perror("Error with opening output file");
            MPI_Finalize();
            exit(EXIT_FAILURE);
        }
        double capacity = CAPACITY_FACTOR * message_size / sum;
        printf("\tMeasured capacity: %f MBit/s\n\n", capacity);
        fprintf(output_file, "%d %f\n", message_size, capacity);
        fclose(output_file);
    }

    fflush(stdout);
    MPI_Finalize();
    
    return 0;
}