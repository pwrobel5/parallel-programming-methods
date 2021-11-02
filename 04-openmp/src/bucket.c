#include <stdio.h>
#include <stdlib.h>

#include "bucket.h"

BucketArray_t *create_bucket_array(int buckets_number) {
    BucketArray_t *result = malloc(sizeof(BucketArray_t));
    if (result == NULL) {
        perror("Error with memory allocation");
        exit(EXIT_FAILURE);
    }

    result->buckets_number = buckets_number;
    result->buckets = malloc(buckets_number * sizeof(Bucket_t*));
    if (result->buckets == NULL) {
        perror("Error with memory allocation");
        exit(EXIT_FAILURE);
    }

    double step = 1.0 / buckets_number;
    double current_min = 0.0;
    for (int i = 0; i < buckets_number; i++) {
        result->buckets[i] = malloc(sizeof(Bucket_t));
        if(result->buckets[i] == NULL) {
            perror("Error with memory allocation");
            exit(EXIT_FAILURE);
        }

        result->buckets[i]->elements = NULL;
        result->buckets[i]->elements_number = 0;
        result->buckets[i]->min_value = current_min;
        result->buckets[i]->max_value = current_min + step;
        current_min += step;
    }

    result->buckets[buckets_number - 1]->max_value = 1.0;
    return result;
}

void delete_bucket_array(BucketArray_t *bucket_array) {
    for (int i = 0; i < bucket_array->buckets_number; i++) {
        Node_t *current_element = bucket_array->buckets[i]->elements;
        Node_t *tmp;
        while(current_element != NULL) {
            tmp = current_element;
            current_element = current_element->next;
            free(tmp);
        }
        free(bucket_array->buckets[i]);
    }

    free(bucket_array->buckets);
    free(bucket_array);
}

void add_to_bucket(Bucket_t *bucket, double value) {
    Node_t *new_node = malloc(sizeof(Node_t));
    if (new_node == NULL) {
        perror("Error with memory allocation");
        exit(EXIT_FAILURE);
    }

    new_node->value = value;
    new_node->next = bucket->elements;
    bucket->elements = new_node;
    bucket->elements_number++;
}

Node_t* sortedInsert(Node_t *node, Node_t *sorted) {
    Node_t *result = sorted;
    if (sorted == NULL || sorted->value >= node->value) {
        node->next = sorted;
        result = node;
    } else {
        Node_t *current = sorted;
        while (current->next != NULL && current->next->value < node->value) {
            current = current->next;
        }
        node->next = current->next;
        current->next = node;
    }

    return result;
}

void sort_bucket(Bucket_t *bucket) {
    Node_t *sorted = NULL;
    Node_t *current_node = bucket->elements;

    while(current_node != NULL) {
        Node_t *next_node = current_node->next;
        sorted = sortedInsert(current_node, sorted);
        current_node = next_node;
    }

    bucket->elements = sorted;
}

void print_bucket(Bucket_t *bucket) {
    Node_t *head = bucket->elements;
    while (head != NULL) {
        printf("%f ", head->value);
        head = head->next;
    }
    printf("\n");
}