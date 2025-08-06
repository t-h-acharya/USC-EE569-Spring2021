#include "hw2.h"

Image *alloc_image(int height, int width, int channels) {
    if ((height <= 0) || (width <= 0) || (channels <= 0)) {
        exit(1);
    }

    Image *image = (Image *)malloc(sizeof(Image));
    image->height = height;
    image->width = width;
    image->channels = channels;
    image->data = (float ***)malloc(height * sizeof(float **));
    if (!image->data) {
        fprintf(stderr, "ERROR: In alloc_image(); Image could not be allocated\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < height; i++) {
        image->data[i] = (float **)malloc(width * sizeof(float *));
        if (!image->data[i]) {
            for (int a = 0; a < i; a++) {
                for (int b = 0; b < width; b++) {
                    free(image->data[a][b]);
                }
                free(image->data[a]);
            }
            free(image->data);
            free(image);
            fprintf(stderr, "ERROR: In alloc_image(); Image could not be allocated\n");
            exit(EXIT_FAILURE);
         }
         for (int j = 0; j < width; j++) {
            image->data[i][j] = (float *)malloc(channels * sizeof(float));
            if (!image->data[i][j]) {
                for (int a = 0; a < i; a++) {
                    for (int b = 0; b < j; b++) {
                        free(image->data[a][b]);
                    }
                    free(image->data[a]);
                }
                free(image->data);
                free(image);
                fprintf(stderr, "ERROR: In alloc_image(); Image could not be allocated\n");
                exit(EXIT_FAILURE);
            }
         }
    }

    return image;
}

Image *copy_image(Image *image) {
    int height = image->height;
    int width = image->width;
    int channels = image->channels;
    Image *copied_image = alloc_image(height, width, channels);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < channels; k++){
                copied_image->data[i][j][k] = image->data[i][j][k];
            }
        }
    }
    return copied_image;
}

void free_image(Image *image) {
    int height = image->height;
    int width = image->width;
    int channels = image->channels;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            free(image->data[i][j]);
        }
        free(image->data[i]);
    }
    free(image->data);
    free(image);
    return;
}

Image *read_image(string filename, int height, int width, int channels) {
    Image *image = alloc_image(height, width, channels);
    FILE *file_ptr = fopen(filename, "rb");
    if (!file_ptr)
    {
        fprintf(stderr, "ERROR: In read_image(); File could not be opened\n");
        exit(EXIT_FAILURE);    
    }

    BYTE temp_buffer;

    for (int i = 0; i < height; i++) {
        for (int j = 0;  j < width; j++) {
            for (int k = 0; k < channels; k++) {
                fread(&temp_buffer, sizeof(BYTE), 1, file_ptr);
                image->data[i][j][k] = temp_buffer;
            }
        }
    }
    return image;
}

void write_image(string filename, Image *image) {
    int height = image->height;
    int width = image->width;
    int channels = image->channels;

    FILE *file_ptr = fopen(filename, "wb");
    if (!file_ptr)
    {
        fprintf(stderr, "ERROR: In write_image(); File could not be opened\n");
        exit(EXIT_FAILURE);    
    }

    BYTE temp_buffer;

    for (int i = 0; i < height; i++) {
        for (int j = 0;  j < width; j++) {
            for (int k = 0; k < channels; k++) {
                temp_buffer = to_BYTE(image->data[i][j][k]);
                fwrite(&temp_buffer, sizeof(BYTE), 1, file_ptr);
            }
        }
    }
    return;
}

BYTE to_BYTE(float value) {
    if (value >= MAX) {
        return MAX;
    }
    else if (value <= MIN)
    {
       return MIN;
    }
    else {
        return round(value);
    }
}