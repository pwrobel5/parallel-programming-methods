#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

#include "bucket.h"

#define CHECK_ARRAY

#define BUCKETS_NUMBER 1000
#define min(a, b) (a < b ? a : b)

double* generate_array(long int array_size) {
    double *result = malloc(array_size * sizeof(double));
    if (result == NULL) {
        perror("Error with memory allocation");
        exit(EXIT_FAILURE);
    }

    #pragma omp parallel
    {
        unsigned int seed = 21 + omp_get_thread_num();
        #pragma omp for schedule(guided, 6000)
        for (int i = 0; i < array_size; i++) {
            result[i] = (rand_r(&seed) * 1.0) / RAND_MAX;
        }
    }

    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Incorrect arguments number, usage: program.x array_size num_threads\n");
        exit(EXIT_FAILURE);
    }

    long int array_size = atol(argv[1]);
    int num_threads = atoi(argv[2]);

    double global_start = omp_get_wtime();
    omp_set_num_threads(num_threads);
    
    double array_generation_start = omp_get_wtime();
    double *array = generate_array(array_size);
    double array_generation_stop = omp_get_wtime();

    BucketArray_t *bucket_array = create_bucket_array(BUCKETS_NUMBER);
    omp_lock_t bucket_locks[BUCKETS_NUMBER];
    for (int i = 0; i < BUCKETS_NUMBER; i++)
        omp_init_lock(bucket_locks + i);

    int elements_per_thread = array_size / num_threads + 1;
    double bucket_division_start = 0.0, bucket_division_stop = 0.0;
    double sort_start = 0.0, sort_stop = 0.0;
    double fill_array_start = 0.0, fill_array_stop = 0.0;

    #pragma omp parallel shared(array, bucket_array, bucket_locks)
    {
        int thread_id = omp_get_thread_num();
        unsigned int start_index = thread_id * elements_per_thread;
        unsigned int end_index = min((thread_id + 1) * elements_per_thread, array_size);

        if (thread_id == 0) bucket_division_start = omp_get_wtime();

        // fill buckets
        for (int i = start_index; i < end_index; i++) {
            double value = array[i];

            int bucket_index = -1;
            for (int j = 0; (j < BUCKETS_NUMBER) && (bucket_index == -1); j++) {
                if (bucket_array->buckets[j]->min_value <= value && bucket_array->buckets[j]->max_value >= value)
                    bucket_index = j;
            }

            omp_set_lock(bucket_locks + bucket_index);
            add_to_bucket(bucket_array->buckets[bucket_index], value);
            omp_unset_lock(bucket_locks + bucket_index);
        }

        #pragma omp barrier
        if (thread_id == 0) bucket_division_stop = sort_start = omp_get_wtime();

        // sorting
        int buckets_per_thread = BUCKETS_NUMBER / num_threads + 1;
        unsigned int sort_start_index = thread_id * buckets_per_thread;
        unsigned int sort_end_index = min((thread_id + 1) * buckets_per_thread, BUCKETS_NUMBER);
        for (int i = sort_start_index; i < sort_end_index; i++) {
            sort_bucket(bucket_array->buckets[i]);
        }

        #pragma omp barrier
        if (thread_id == 0) sort_stop = fill_array_start = omp_get_wtime();

        // get the index of first value to put into array
        int fill_start_index = 0;
        for (int i = 0; i < sort_start_index; i++) {
            fill_start_index += bucket_array->buckets[i]->elements_number;;
        }

        int shift = 0;
        for (int i = sort_start_index; i < sort_end_index; i++) {
            Node_t *current_node = bucket_array->buckets[i]->elements;
            while (current_node != NULL) {
                array[fill_start_index + shift] = current_node->value;
                current_node = current_node->next;
                shift++;
            }
        }

        #pragma omp barrier
        if (thread_id == 0) fill_array_stop = omp_get_wtime();
    }
    double global_stop = omp_get_wtime();

    #ifdef CHECK_ARRAY
    
    for (int i = 0; i < array_size - 1; i++) {        
        if (array[i] > array[i + 1])
            printf("Incorrect order: %f before %f\n", array[i], array[i + 1]);
    }
    printf("\n");

    #endif
    
    for (int i = 0; i < BUCKETS_NUMBER; i++)
        omp_destroy_lock(bucket_locks + i);

    delete_bucket_array(bucket_array);
    free(array);

    printf("Time report:\n");
    printf("\tRandom array generation:\t\t%f s\n", array_generation_stop - array_generation_start);
    printf("\tDivision into buckets:\t\t\t%f s\n", bucket_division_stop - bucket_division_start);
    printf("\tSorting buckets:\t\t\t%f s\n", sort_stop - sort_start);
    printf("\tFilling array with sorted values:\t%f s\n", fill_array_stop - fill_array_start);
    printf("\tTotal:\t\t\t\t\t%f s\n\n", global_stop - global_start);

    return 0;
}