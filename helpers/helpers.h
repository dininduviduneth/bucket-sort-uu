// helpers.h.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct
{
    double *array;
    int array_size;
} ArrayData;

void generate_uniform_array(double min, double max, double numbers[], int array_length);
void generate_file(double *arr, int array_length, char *output_path);
void read_file(char *input_path, ArrayData *array_data);

#endif
