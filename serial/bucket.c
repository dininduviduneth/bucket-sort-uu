#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../helpers/helpers.h"

int main(int argc, char *argv[]) {
    /* Validate if the user entered the correct arguments */
    if (argc != 2)
    {
        printf("Incorrect number of arguments!\n");
        printf("Usage: %s input_path\n", argv[0]);
        return 0;
    }

    char *input_path = argv[1];

    ArrayData *array_data = malloc(sizeof(ArrayData));
    if (array_data == NULL)
    {
        printf("Failed to allocate memory for array_data.\n");
        return 0;
    }
    
    read_file(input_path, array_data);

    for(int i = 0; i < array_data->array_size; i++) {
        printf("%f ", array_data->array[i]);
    }
    printf("\n\n");

    printf("Array size is: %d\n", array_data->array_size);

    /* START - Identifying the min and max values of the array */
    double min_val, max_val = 0.0;
    for(int i = 0; i < array_data->array_size; i++) {
        if(array_data->array[i] < min_val) {
            min_val = array_data->array[i];
        } else if(array_data->array[i] > max_val) {
            max_val = array_data->array[i];
        }
    }
    printf("Min value: %f, Max value: %f\n\n", min_val, max_val);
    /* END - Identifying the min and max values of the array */

    BucketData *bucket_data = bucket_split(array_data, min_val, max_val, 8);
    
    printf("BEFORE QUICK SORT: \n");
    for(int i = 0; i < 8; i++) {
        printf("Split %d: ", i + 1);
        for(int j = 0; j < bucket_data->bucket_filled_count[i]; j++) {
            printf("%f ", bucket_data->buckets[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for(int i = 0; i < 8; i++) {
        quicksort(bucket_data->buckets[i], 0, bucket_data->bucket_filled_count[i] - 1);
    }

    printf("AFTER QUICK SORT: \n");
    for(int i = 0; i < 8; i++) {
        printf("Split %d: ", i + 1);
        for(int j = 0; j < bucket_data->bucket_filled_count[i]; j++) {
            printf("%f ", bucket_data->buckets[i][j]);
        }
        printf("\n\n");
    }

    merge_buckets(bucket_data, array_data);
    printf("\n");

    printf("Sorted array: ");
    for(int i = 0; i < array_data->array_size; i++) {
        printf("%f ", array_data->array[i]);
    }
    printf("\n");

    generate_file(array_data->array, array_data->array_size, "result.bin");

    // Free allocated memory in the array
    free(array_data->array);
    // Free allocated memory in array_data
    free(array_data);

    // Free memory for each bucket
    for (int i = 0; i < 8; i++) {
        free(bucket_data->buckets[i]);
    }
    // Free memory for the array of buckets
    free(bucket_data->buckets);
    // Free memory for bucket_filled_count array
    free(bucket_data->bucket_filled_count);
    // Free memory for the BucketData structure
    free(bucket_data);
    
    return 0;
}