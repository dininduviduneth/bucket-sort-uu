#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <omp.h>
#include <sys/time.h>

// helpers
#include "helpers.h"

// Change the VERSION to 2 to check the median-of-three pivot selection strategy
#define VERSION 1

void generate_uniform_array(double min, double max, double *numbers, int array_length) {
    for (int i = 0; i < array_length; i++) {
        double random_value = ((double)rand() / RAND_MAX) * (max - min) + min;
        numbers[i] = random_value;
    }
}

void generate_normal_array(double min, double max, double *numbers, int array_length) {
    double mean = min + (max - min) / 2;
    double stdev = (max - min) / 6;
    for(int i = 0; i < array_length; i++) {
        double u1 = rand() / (RAND_MAX + 1.0);
        double u2 = rand() / (RAND_MAX + 1.0);

        double n = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
        double normal_value = mean + stdev * n;
        numbers[i] = normal_value;
    }
}

void generate_exponential_array(double min, double max, double *numbers, int array_length) {
    double lambda = 1 / (max - min);

    for(int i = 0; i < array_length; i++) {
        double u = rand() / (RAND_MAX + 1.0);

        double exponential_value = -log(1 - u) / lambda;
        numbers[i] = exponential_value;
    }
}

void generate_file(double *arr, int array_length, char *output_path)
{
    FILE *output_file = fopen(output_path, "wb");
    if (!output_file)
    {
        printf("Failed to open the output file.\n");
        return;
    }

    // Write the array length
    fwrite(&(array_length), sizeof(int), 1, output_file);

    for (int i = 0; i < array_length; i++)
    {
        fwrite(&(arr[i]), sizeof(double), 1, output_file);
    }
    fclose(output_file);
}


void read_file(char *input_path, ArrayData *array_data)
{
    /* Open input file and determine its size. */
    FILE *input_file = fopen(input_path, "rb");

    if (!input_file)
    {
        printf("read_doubles_from_file error: failed to open input file '%s'.\n", input_path);
        return;
    }

    int array_size;
    size_t read_size = fread(&array_size, sizeof(int), 1, input_file);

    if (read_size != 1)
    {
        printf("read_file error: failed to read array size.\n");
        fclose(input_file);
        return;
    }

    array_data->array_size = array_size;
    array_data->array = (double *)malloc(array_size * sizeof(double));
    if (array_data->array == NULL)
    {
        printf("read_file error: failed to allocate memory for array.\n");
        fclose(input_file);
        return;
    }

    size_t elements_read = fread(array_data->array, sizeof(double), array_data->array_size, input_file);

    if (elements_read != (size_t)array_data->array_size)
    {
        printf("read_file error: failed to read array elements.\n");
        fclose(input_file);
        return;
    }

    fclose(input_file);
}

BucketData *bucket_split(ArrayData *array_data, double min_val, double max_val, int bucket_count) {
    double bucket_count_recip = 1 / ((double)bucket_count);
    BucketData *bucket_data = malloc(sizeof(BucketData));

    double bucket_length = (max_val - min_val) * bucket_count_recip;
    int elements_per_bucket = (array_data->array_size * bucket_count_recip) + 1;

    // Creating an array of buckets
    double **buckets = (double **)malloc(bucket_count * sizeof(double *));
    int bucket_limit[bucket_count];
    int *bucket_filled_count = malloc(bucket_count * sizeof(int));

    // Assign the elements per bucket
    for (int i = 0; i < bucket_count; i++) {
        bucket_limit[i] = elements_per_bucket; // Initially we assign the same number of elements for each bucket
        bucket_filled_count[i] = 0;
    }

    for(int i = 0; i < bucket_count; i++) {
        buckets[i] = (double *)malloc(elements_per_bucket * sizeof(double));
    }

    // Send values to buckets
    double bucket_length_recip = 1 / ((double)bucket_length);
    for(int i = 0; i < array_data->array_size; i++) {
        int target_bucket_id = (array_data->array[i] - min_val) * bucket_length_recip;

        // To make the max element fall in to the last bucket
        if(target_bucket_id == bucket_count) {
            target_bucket_id--;
        }

        // If the a bucket is full, we reallocate the bucket to fill more
        if(bucket_filled_count[target_bucket_id] >= bucket_limit[target_bucket_id]) {
            bucket_limit[target_bucket_id] += (elements_per_bucket * bucket_count_recip);
            buckets[target_bucket_id] = (double *)realloc(buckets[target_bucket_id], bucket_limit[target_bucket_id] * sizeof(double));
            printf("Reallocation had to run for bucket: %d\n", target_bucket_id);
        }

        // Assign the value from the array to the selected position in the selected bucket
        buckets[target_bucket_id][bucket_filled_count[target_bucket_id]] = array_data->array[i];
        bucket_filled_count[target_bucket_id]++;
    }

    bucket_data->bucket_filled_count = bucket_filled_count;
    bucket_data->buckets = buckets;

    return bucket_data;
}

void quicksort(double *arr, int start_index, int end_index) {
    if(start_index >= end_index) {
        return;
    }

    #if VERSION == 1
    int pivot_index = start_index + (end_index - start_index) / 2;

    #elif VERSION == 2
    int pivot_index;
    int middle_index = start_index + (end_index - start_index) / 2;

    if (arr[start_index] < arr[middle_index]) {
        if (arr[middle_index] < arr[end_index]) {
            pivot_index = middle_index;
        } else if (arr[start_index] < arr[end_index]) {
            pivot_index = end_index;
        } else {
            pivot_index = start_index;
        }
    } else {
        if (arr[start_index] < arr[end_index]) {
            pivot_index = start_index;
        } else if (arr[middle_index] < arr[end_index]) {
            pivot_index = end_index;
        } else {
            pivot_index = middle_index;
        }
    }
    #endif

    double pivot = arr[pivot_index];

    int i = start_index;
    int j = end_index;

    while(i <= j) {
        while(arr[i] < pivot) {
            i++;
        }

        while(arr[j] > pivot) {
            j--;
        }

        if(i <= j) {
            swap_numbers(&arr[i], &arr[j]);
            i++;
            j--;
        }
    }

    quicksort(arr, start_index, j);
    quicksort(arr, i, end_index);
}

void swap_numbers(double *num1, double *num2) {
    double temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

void merge_buckets(BucketData *bucket_data, ArrayData *array_data, int bucket_count) {
    // Check if original array size and total of bucket item count are matching
    int total_numbers_in_buckets = 0;
    for(int i = 0; i < bucket_count; i++) {
        total_numbers_in_buckets += bucket_data->bucket_filled_count[i];
        printf("Numbers in bucket %d: %d\n", i, bucket_data->bucket_filled_count[i]);
    }
    printf("Total numbers in buckets: %d\n", total_numbers_in_buckets);
    printf("Total numbers in array: %d\n", array_data->array_size);

    if(total_numbers_in_buckets != array_data->array_size) {
        printf("Count of numbers in the array and all buckets doesn't match, something has gone wrong!\n");
        return;
    }

    int array_write_pointer = 0;
    for(int i = 0; i < bucket_count; i++) {
        for(int j = 0; j < bucket_data->bucket_filled_count[i]; j++) {
            array_data->array[array_write_pointer] = bucket_data->buckets[i][j];
            array_write_pointer++;
        }
    }

    if(array_write_pointer == array_data->array_size){
        printf("Merged all buckets to the array!\n");
    } else {
        printf("There was an error in merging! Last write point: %d\n", array_write_pointer);
    }
}

void merge_buckets_parallel(BucketData *bucket_data, ArrayData *array_data, int bucket_count) {
    // Check if original array size and total of bucket item count are matching
    int total_numbers_in_buckets = 0;
    for(int i = 0; i < bucket_count; i++) {
        total_numbers_in_buckets += bucket_data->bucket_filled_count[i];
        printf("Numbers in bucket %d: %d\n", i, bucket_data->bucket_filled_count[i]);
    }
    printf("Total numbers in buckets: %d\n", total_numbers_in_buckets);
    printf("Total numbers in array: %d\n", array_data->array_size);

    if(total_numbers_in_buckets != array_data->array_size) {
        printf("Count of numbers in the array and all buckets doesn't match, something has gone wrong!\n");
        return;
    }

    int start_bucket_index[bucket_count];
    int end_bucket_index[bucket_count];

    for(int i = 0; i < bucket_count; i++) {
        if(i == 0) {
            start_bucket_index[i] = 0;
            end_bucket_index[i] = bucket_data->bucket_filled_count[i];
        } else {
            start_bucket_index[i] = start_bucket_index[i - 1] + bucket_data->bucket_filled_count[i - 1];
            end_bucket_index[i] = end_bucket_index[i - 1] + bucket_data->bucket_filled_count[i];
        }

        // Use the below code for varification
        // printf("Index: %d --> [%d, %d]\n", i, start_bucket_index[i], end_bucket_index[i]);
    }

    #pragma omp parallel for num_threads(bucket_count)
    for(int i = 0; i < bucket_count; i++) {
        for(int j = start_bucket_index[i]; j < end_bucket_index[i]; j++) {
            array_data->array[j] = bucket_data->buckets[i][j - start_bucket_index[i]];
        }
    }
}

double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}