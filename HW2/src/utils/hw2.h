#ifndef HW2_H
#define HW2_H

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

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

// Constants
const BYTE MAX = UINT8_MAX;
const BYTE MIN = 0;

// Functions
Image *alloc_image(int height, int width, int channels);
Image *copy_image(Image *image);
void free_image(Image *image);

Image *read_image(string filename, int height, int width, int channels);
void write_image(string filename, Image *image);

BYTE to_BYTE(float value);
#endif