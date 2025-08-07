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
} Image;

// Functions
Image *alloc_image(int height, int width, int channels);
Image *copy_image(Image *image);
void free_image(Image *image);

Image *read_image(string filename, int height, int width, int channels);
void write_image(string filename, Image *image);

BYTE to_BYTE(float value);
#endif