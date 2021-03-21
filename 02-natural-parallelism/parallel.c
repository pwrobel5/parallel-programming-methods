#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <mpi/mpi.h>

#define DESIRED_ARGC 3
#define POINTS_NUMBER_POSITION 1
#define OUTPUT_FILE_POSITION 2
#define CIRCLE_RADIUS 1.0
#define RANDOM_SEED 2137
#define PI (atan(1.0) * 4.0)

int is_point_in_circle(double x, double y)
{
    double distance = sqrt(pow(x, 2.0) + pow(y, 2.0));
    return distance <= CIRCLE_RADIUS ? 1 : 0;
}

int main(int argc, char* argv[])
{
    if(argc != DESIRED_ARGC)
    {
        errno = EINVAL;
        perror("Incorrect number of initial arguments, needed 2");
        exit(EXIT_FAILURE);
    }

    unsigned long long int global_points_number = atoll(argv[POINTS_NUMBER_POSITION]);
    char* output_file_name = argv[OUTPUT_FILE_POSITION];
    double t_start = 0.0;
    double t_stop = 0.0;
    int rank, num_proc;
    unsigned long long int local_points_in_circle = 0l;
    unsigned long long int global_points_in_circle = 0l;
    srand(RANDOM_SEED);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    unsigned long long int local_points_number = global_points_number / num_proc;
    MPI_Barrier(MPI_COMM_WORLD);

    t_start = MPI_Wtime();
    for(unsigned long long int i = 0; i < local_points_number; i++)
    {
        double x = (double) rand() / RAND_MAX;
        double y = (double) rand() / RAND_MAX;
        if(is_point_in_circle(x, y) == 1) local_points_in_circle++;
    }

    MPI_Reduce(&local_points_in_circle, &global_points_in_circle, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if(rank == 0)
    {
        long double pi_approximation = 4.0 * (long double) global_points_in_circle / global_points_number;
        t_stop = MPI_Wtime();
        double time_in_s = t_stop - t_start;
        printf("PI approximation: %Lf\n", pi_approximation);
        printf("Time: %f s\n", time_in_s);

        FILE* output_file = fopen(output_file_name, "a");
        if(output_file == NULL)
        {
            perror("Error with opening output file");
            MPI_Finalize();
            exit(EXIT_FAILURE);
        }

        fprintf(output_file, "%d %lld %f\n", num_proc, global_points_number, time_in_s);
        fclose(output_file);
    }

    MPI_Finalize();

    return 0;
}