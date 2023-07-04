#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../helpers/helpers.h"

int main(int argc, char *argv[]) {
    /* Validate if the user entered the correct arguments */
    if (argc != 4)
    {
        printf("Incorrect number of arguments!\n");
        printf("Usage: %s array_length distribution output_filename\n", argv[0]);
        return 0;
    }

    // Get the array length from the user
    int array_length = atoi(argv[1]);
    char *distribution = argv[2];
    char *output_filename = argv[3];

    // Set up initial seed - to make the number generation consistent
    srand(10);

    // double numbers[array_length];
    double *numbers = (double *)malloc(array_length * sizeof(double));

    printf("Passed this point for array size %d\n", array_length);

    if(distribution[0] == 'U') {
        generate_uniform_array(-1 * array_length, array_length, numbers, array_length);
    } else if(distribution[0] == 'N') {
        generate_normal_array(-1 * array_length, array_length, numbers, array_length);
    } else if(distribution[0] == 'E') {
        generate_exponential_array(1, array_length, numbers, array_length);
    } else {
        printf("Enter a valid distribution. Either U - Uniform, N - Normal or E - Exponential!\n");
        return 0;
    }

    // PRINT GENERATED ARRAY
    // for(int i = 0; i < array_length; i++) {
    //     printf("%f ", numbers[i]);
    // }
    // printf("\n");

    printf("Array size is: %d\n", array_length);

    generate_file(numbers, array_length, output_filename);

    free(numbers);
    
    return 0;
}