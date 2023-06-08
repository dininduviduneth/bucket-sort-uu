// helpers.h.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct
{
    double *array;
    int array_size;
} ArrayData;

typedef struct
{
    double **buckets;
    int *bucket_filled_count;
} BucketData;

void generate_uniform_array(double min, double max, double numbers[], int array_length);
void generate_file(double *arr, int array_length, char *output_path);
void read_file(char *input_path, ArrayData *array_data);
BucketData *bucket_split(ArrayData *array_data, double min_val, double max_val, int bucket_count);
void quicksort(double *arr, int start_index, int end_index);
void swap_numbers(double *num1, double *num2);
void merge_buckets(BucketData *bucket_data, ArrayData *array_data);
double get_wall_seconds();
void generate_normal_array(double min, double max, double numbers[], int array_length);
void generate_exponential_array(double min, double max, double numbers[], int array_length);

#endif
