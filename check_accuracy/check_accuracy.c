#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../helpers/helpers.h"

int main(int argc, char *argv[]) {
    /* Validate if the user entered the correct arguments */
    if (argc != 3)
    {
        printf("Incorrect number of arguments!\n");
        printf("Usage: %s results_path input_path\n", argv[0]);
        return 0;
    }

    char *results_path = argv[1];
    char *input_path = argv[2];

    ArrayData *results_array_data = malloc(sizeof(ArrayData));
    if (results_array_data == NULL)
    {
        printf("Failed to allocate memory for array_data.\n");
        return 0;
    }

    ArrayData *input_array_data = malloc(sizeof(ArrayData));
    if (input_array_data == NULL)
    {
        printf("Failed to allocate memory for array_data.\n");
        return 0;
    }

    read_file(results_path, results_array_data);
    read_file(input_path, input_array_data);
    
    if(results_array_data->array_size != input_array_data->array_size) {
        printf("The array sizes doesn't match!\n");
        
        // Free allocated memory in the array
        free(results_array_data->array);
        free(input_array_data->array);
        // Free allocated memory in array_data
        free(results_array_data);
        free(input_array_data);
        return 0;
    }

    double results_sum = 0, input_sum = 0;

    for(int i = 0; i < results_array_data->array_size; i++) {
        results_sum += results_array_data->array[i];
        input_sum += input_array_data->array[i];
    }

    if(fabs(results_sum - input_sum) > 0.000001) {
        printf("The array sums doesn't match!\n");
        printf("Results Sum: %f\n", results_sum);
        printf("Input Sum: %f\n", input_sum);
        printf("Difference: %f\n", results_sum - input_sum);
    }

    for(int i = 0; i < results_array_data->array_size - 1; i++) {
        if(results_array_data->array[i + 1] < results_array_data->array[i]) {
            printf("index[%d] = %f < index[%d] = %f\n", i, results_array_data->array[i], i + 1, results_array_data->array[i + 1]);
            // Free allocated memory in the array
            free(results_array_data->array);
            free(input_array_data->array);
            // Free allocated memory in array_data
            free(results_array_data);
            free(input_array_data);
            
            break;
        }
    }

    printf("The results file has perfectly sorted values!\n");
    
    // Free allocated memory in the array
    free(results_array_data->array);
    free(input_array_data->array);
    // Free allocated memory in array_data
    free(results_array_data);
    free(input_array_data);

    return 0;
}