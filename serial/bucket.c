#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
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
    
    double full_run_start_time = get_wall_seconds();
    double read_file_start_time = get_wall_seconds();
    read_file(input_path, array_data);
    double read_file_end_time = get_wall_seconds();

    // PRINT LOADED ARRAY
    // for(int i = 0; i < array_data->array_size; i++) {
    //     printf("%f ", array_data->array[i]);
    // }
    // printf("\n\n");

    printf("Array size is: %d\n", array_data->array_size);

    /* START - Identifying the min and max values of the array */
    double min_max_start_time = get_wall_seconds();
    double min_val, max_val = 0.0;
    for(int i = 0; i < array_data->array_size; i++) {
        if(array_data->array[i] < min_val) {
            min_val = array_data->array[i];
        } else if(array_data->array[i] > max_val) {
            max_val = array_data->array[i];
        }
    }
    printf("Min value: %f, Max value: %f\n\n", min_val, max_val);
    double min_max_end_time = get_wall_seconds();
    /* END - Identifying the min and max values of the array */

    /* START - Splitting the numbers into buckets */
    double bucket_split_start_time = get_wall_seconds();
    BucketData *bucket_data = bucket_split(array_data, min_val, max_val, 8);
    double bucket_split_end_time = get_wall_seconds();
    /* END - Splitting the numbers into buckets */
    
    // PRINT BUCKETS BEFORE QUICK SORT
    // printf("BEFORE QUICK SORT: \n");
    // for(int i = 0; i < 8; i++) {
    //     printf("Split %d: ", i + 1);
    //     for(int j = 0; j < bucket_data->bucket_filled_count[i]; j++) {
    //         printf("%f ", bucket_data->buckets[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    /* START - Running bucket-wise Quick Sort */
    double quicksort_start_time = get_wall_seconds();
    for(int i = 0; i < 8; i++) {
        quicksort(bucket_data->buckets[i], 0, bucket_data->bucket_filled_count[i] - 1);
    }
    double quicksort_end_time = get_wall_seconds();
    /* END - Running bucket-wise Quick Sort */

    // PRINT BUCKETS AFTER QUICK SORT
    // printf("AFTER QUICK SORT: \n");
    // for(int i = 0; i < 8; i++) {
    //     printf("Split %d: ", i + 1);
    //     for(int j = 0; j < bucket_data->bucket_filled_count[i]; j++) {
    //         printf("%f ", bucket_data->buckets[i][j]);
    //     }
    //     printf("\n\n");
    // }

    /* START - Merging buckets to original array */
    double merge_start_time = get_wall_seconds();
    merge_buckets(bucket_data, array_data);
    double merge_end_time = get_wall_seconds();
    /* END - Merging buckets to original array */

    // PRINT SORTED ARRAY
    // printf("\n");
    // printf("Sorted array: ");
    // for(int i = 0; i < array_data->array_size; i++) {
    //     printf("%f ", array_data->array[i]);
    // }
    // printf("\n");

    /* START - Save sorted array in file */
    double save_data_start_time = get_wall_seconds();
    generate_file(array_data->array, array_data->array_size, "result.bin");
    double save_data_end_time = get_wall_seconds();
    /* START - Save sorted array in file */
    double full_run_end_time = get_wall_seconds();

    // Print time measurements
    printf("\n");
    printf("Time taken for reading file: %f%%\n", (min_max_end_time - min_max_start_time) * 100 / (full_run_end_time - full_run_start_time));
    printf("Time taken for min-max identifier: %f%%\n", (read_file_end_time - read_file_start_time) * 100 / (full_run_end_time - full_run_start_time));
    printf("Time taken for bucket split: %f%%\n", (bucket_split_end_time - bucket_split_start_time) * 100 / (full_run_end_time - full_run_start_time));
    printf("Time taken for quick sort: %f%%\n", (quicksort_end_time - quicksort_start_time) * 100 / (full_run_end_time - full_run_start_time));
    printf("Time taken for bucket merge: %f%%\n", (merge_end_time - merge_start_time) * 100 / (full_run_end_time - full_run_start_time));
    printf("Time taken for saving data to file: %f%%\n", (save_data_end_time - save_data_start_time) * 100 / (full_run_end_time - full_run_start_time));
    printf("Time aggregation: %f%%\n", ((min_max_end_time - min_max_start_time) + (read_file_end_time - read_file_start_time) + (bucket_split_end_time - bucket_split_start_time) + (quicksort_end_time - quicksort_start_time) + (merge_end_time - merge_start_time) + (save_data_end_time - save_data_start_time)) * 100 / (full_run_end_time - full_run_start_time));
    printf("Total time taken: %f seconds\n\n", full_run_end_time - full_run_start_time);

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