#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// helpers
#include "helpers.h"

void generate_uniform_array(double min, double max, double numbers[], int array_length) {
    for (int i = 0; i < array_length; i++) {
        double random_value = ((double)rand() / RAND_MAX) * (max - min) + min;
        numbers[i] = random_value;
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
    fread(&array_size, sizeof(int), 1, input_file);

    array_data->array_size = array_size;
    array_data->array = (double *)malloc(array_size * sizeof(double));
    if (array_data->array == NULL)
    {
        printf("read_file error: failed to allocate memory for array.\n");
        fclose(input_file);
        return;
    }

    fread(array_data->array, sizeof(double), array_data->array_size, input_file);

    fclose(input_file);
}

BucketData *bucket_split(ArrayData *array_data, double min_val, double max_val, int bucket_count) {
    BucketData *bucket_data = malloc(sizeof(BucketData));

    double bucket_length = (max_val - min_val) / bucket_count;
    int elements_per_bucket = (array_data->array_size / bucket_count) + 1;

    // Creating an array of buckets
    double **buckets = (double **)malloc(bucket_count * sizeof(double *));
    int bucket_limit[bucket_count];
    int *bucket_filled_count = malloc(bucket_count * sizeof(int));

    // Assign the elements per bucket
    for (int i = 0; i < bucket_count; i++) {
        bucket_limit[i] = elements_per_bucket;
        bucket_filled_count[i] = 0;
    }

    for(int i = 0; i < bucket_count; i++) {
        buckets[i] = (double *)malloc(elements_per_bucket * sizeof(double));
    }

    // Send values to buckets
    for(int i = 0; i < array_data->array_size; i++) {
        int target_bucket_id = (array_data->array[i] - min_val) / bucket_length;

        if(target_bucket_id == bucket_count) {
            target_bucket_id--;
        }

        if(bucket_filled_count[target_bucket_id] >= bucket_limit[target_bucket_id]) {
            bucket_limit[target_bucket_id] += (elements_per_bucket / 8);
            buckets[target_bucket_id] = (double *)realloc(buckets[target_bucket_id], bucket_limit[target_bucket_id] * sizeof(double));
        }

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

    int pivot_index = start_index + (end_index - start_index) / 2;
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

void merge_buckets(BucketData *bucket_data, ArrayData *array_data) {
    // Check if original array size and total of bucket item count are matching
    int total_numbers_in_buckets = 0;
    for(int i = 0; i < 8; i++) {
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
    for(int i = 0; i < 8; i++) {
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