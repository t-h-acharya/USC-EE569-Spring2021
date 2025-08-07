#ifndef HW2_H
#define HW2_H

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// Constants
#define MAX_INTENSITY UINT8_MAX
#define MIN_INTENSITY 0

// Typedef
typedef char* string;
typedef uint8_t BYTE;
typedef struct 
{
    int height;
    int width;
    int channels;
    float ***data;
} Matrix;

// Functions
Matrix *alloc_matrix(int height, int width, int channels);
Matrix *copy_matrix(Matrix *image);
void free_matrix(Matrix *image);

Matrix *read_matrix(string filename, int height, int width, int channels);
void write_matrix(string filename, Matrix *image);

BYTE to_BYTE(float value);

Matrix *matrix_padding(Matrix *image, Matrix *filter);
Matrix *raster_convolution(Matrix *image, Matrix *filter);
Matrix *serpentine_convolution(Matrix *image, Matrix *filter);

#endif