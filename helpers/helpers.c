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
