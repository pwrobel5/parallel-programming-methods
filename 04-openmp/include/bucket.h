#ifndef BUCKET_H
#define BUCKET_H

typedef struct Node {
    double value;
    struct Node *next;
} Node_t;

typedef struct Bucket {
    Node_t *elements;
    int elements_number;
    double min_value;
    double max_value;
} Bucket_t;

typedef struct BucketArray {
    Bucket_t **buckets;
    int buckets_number;
} BucketArray_t;

BucketArray_t *create_bucket_array(int buckets_number);
void delete_bucket_array(BucketArray_t *bucket_array);

void add_to_bucket(Bucket_t *bucket, double value);
void sort_bucket(Bucket_t *bucket);
void print_bucket(Bucket_t *bucket);

#endif