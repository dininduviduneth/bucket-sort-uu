#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>
#include "../helpers/helpers.h"

int main(int argc, char *argv[]) {

    /* Validate if the user entered the correct arguments */
    if (argc != 3)
    {
        printf("Incorrect number of arguments!\n");
        printf("Usage: %s input_path thread_count\n", argv[0]);
        return 0;
    }

    char *input_path = argv[1];
    int thread_count = atoi(argv[2]);
    int bucket_count = thread_count;

    ArrayData *array_data = malloc(sizeof(ArrayData));
    if (array_data == NULL)
    {
        printf("Failed to allocate memory for array_data.\n");
        return 0;
    }
    
    read_file(input_path, array_data);

    // PRINT LOADED ARRAY
    // for(int i = 0; i < array_data->array_size; i++) {
    //     printf("%f ", array_data->array[i]);
    // }
    // printf("\n\n");

    // printf("Array size is: %d\n", array_data->array_size);

    double bucketsort_start_time = omp_get_wtime();
    /* START - Identifying the min and max values of the array */
    double min_max_start_time = omp_get_wtime();
    double min_val = 0.0;
    double max_val = 0.0;
    for(int i = 0; i < array_data->array_size; i++) {
        if(array_data->array[i] < min_val) {
            min_val = array_data->array[i];
        } else if(array_data->array[i] > max_val) {
            max_val = array_data->array[i];
        }
    }
    printf("Min value: %f, Max value: %f\n\n", min_val, max_val);
    double min_max_end_time = omp_get_wtime();
    /* END - Identifying the min and max values of the array */

    /* START - Splitting the numbers into buckets */
    double bucket_split_start_time = omp_get_wtime();
    BucketData *bucket_data = bucket_split(array_data, min_val, max_val, bucket_count);
    double bucket_split_end_time = omp_get_wtime();
    /* END - Splitting the numbers into buckets */
    
    // PRINT BUCKETS BEFORE QUICK SORT
    // printf("BEFORE QUICK SORT: \n");
    // for(int i = 0; i < bucket_count; i++) {
    //     printf("Split %d: ", i + 1);
    //     for(int j = 0; j < bucket_data->bucket_filled_count[i]; j++) {
    //         printf("%f ", bucket_data->buckets[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    /* START - Running bucket-wise Quick Sort */
    double quicksort_start_time = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, 1) num_threads(bucket_count)
    for(int i = 0; i < bucket_count; i++) {
        quicksort(bucket_data->buckets[i], 0, bucket_data->bucket_filled_count[i] - 1);
    }
    double quicksort_end_time = omp_get_wtime();
    /* END - Running bucket-wise Quick Sort */

    // PRINT BUCKETS AFTER QUICK SORT
    // printf("AFTER QUICK SORT: \n");
    // for(int i = 0; i < bucket_count; i++) {
    //     printf("Split %d: ", i + 1);
    //     for(int j = 0; j < bucket_data->bucket_filled_count[i]; j++) {
    //         printf("%f ", bucket_data->buckets[i][j]);
    //     }
    //     printf("\n\n");
    // }

    /* START - Merging buckets to original array */
    double merge_start_time = omp_get_wtime();
    merge_buckets_parallel(bucket_data, array_data, bucket_count);
    double merge_end_time = omp_get_wtime();
    /* END - Merging buckets to original array */
    double bucketsort_end_time = omp_get_wtime();

    // PRINT SORTED ARRAY
    // printf("\n");
    // printf("Sorted array: \n");
    // for(int i = 0; i < array_data->array_size; i++) {
    //     printf("%f ", array_data->array[i]);
    // }
    // printf("\n");

    /* START - Save sorted array in file */
    generate_file(array_data->array, array_data->array_size, "result.bin");
    /* START - Save sorted array in file */

    // Print time measurements
    printf("\n");
    printf("%% Time taken for min-max identifier: %f seconds, %f%%\n", (min_max_end_time - min_max_start_time), (min_max_end_time - min_max_start_time) * 100 / (bucketsort_end_time - bucketsort_start_time));
    printf("%% Time taken for bucket split: %f seconds, %f%%\n", (bucket_split_end_time - bucket_split_start_time), (bucket_split_end_time - bucket_split_start_time) * 100 / (bucketsort_end_time - bucketsort_start_time));
    printf("%% Time taken for quick sort: %f seconds, %f%%\n", (quicksort_end_time - quicksort_start_time), (quicksort_end_time - quicksort_start_time) * 100 / (bucketsort_end_time - bucketsort_start_time));
    printf("%% Time taken for bucket merge: %f seconds, %f%%\n", (merge_end_time - merge_start_time), (merge_end_time - merge_start_time) * 100 / (bucketsort_end_time - bucketsort_start_time));
    printf("Total time taken: %f seconds\n\n", (bucketsort_end_time - bucketsort_start_time));

    // Free allocated memory in the array
    free(array_data->array);
    // Free allocated memory in array_data
    free(array_data);

    // Free memory for each bucket
    for (int i = 0; i < bucket_count; i++) {
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