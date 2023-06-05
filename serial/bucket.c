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
    printf("\n");

    printf("Array size is: %d\n", array_data->array_size);

    generate_file(array_data->array, array_data->array_size, "result.bin");

    // Free allocated memory in the array
    free(array_data->array);
    // Free allocated memory in array_data
    free(array_data);
    
    return 0;
}