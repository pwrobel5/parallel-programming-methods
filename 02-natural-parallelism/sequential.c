#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#define DESIRED_ARGC 2
#define POINTS_NUMBER_POSITION 1
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
        perror("Incorrect number of initial arguments, needed 1");
        exit(EXIT_FAILURE);
    }

    unsigned long long int points_number = atoll(argv[POINTS_NUMBER_POSITION]);
    unsigned long long int points_in_circle = 0l;
    printf("Given points: %llu\n", points_number);
    
    srand(RANDOM_SEED);
    for(unsigned long long int i = 0; i < points_number; i++)
    {
        double x = (double) rand() / RAND_MAX;
        double y = (double) rand() / RAND_MAX;
        if(is_point_in_circle(x, y) == 1) points_in_circle++;
    }

    long double pi_approximation = 4.0 * (long double) points_in_circle / points_number;
    printf("PI approximation: %Lf\n", pi_approximation);

    return 0;
}