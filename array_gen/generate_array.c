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
        printf("Usage: %s array_length\n", argv[0]);
        return 0;
    }

    // Get the array length from the user
    int array_length = atoi(argv[1]);

    // Set up initial seed - to make the number generation consistent
    srand(10);

    double numbers[array_length];
    generate_uniform_array(-10000, 10000, numbers, array_length);

    for(int i = 0; i < array_length; i++) {
        printf("%f ", numbers[i]);
    }
    printf("\n");

    int length = sizeof(numbers) / sizeof(numbers[0]);
    printf("Array size is: %d\n", length);

    generate_file(numbers, array_length, "result.bin");
    
    return 0;
}