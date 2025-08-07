#include "hw2.h"

Matrix *alloc_matrix(int height, int width, int channels) {
    if ((height <= 0) || (width <= 0) || (channels <= 0)) {
        fprintf(stderr, "ERROR: alloc_matrix() \n\t Invalid Matrix Dimensions\n");
        return NULL;
    }

    Matrix *image = (Matrix *)malloc(sizeof(Matrix));
    image->height = height;
    image->width = width;
    image->channels = channels;
    image->data = (float ***)malloc(height * sizeof(float **));
    if (!image->data) {
        fprintf(stderr, "ERROR: alloc_matrix() \n\t Matrix could not be allocated\n");
        return NULL;
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
            fprintf(stderr, "ERROR: alloc_matrix() \n\t Matrix could not be allocated\n");
            return NULL;
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
                fprintf(stderr, "ERROR: alloc_matrix() \n\t Matrix could not be allocated\n");
                return NULL;
            }
         }
    }

    return image;
}

Matrix *copy_matrix(Matrix *image) {
    int height = image->height;
    int width = image->width;
    int channels = image->channels;
    Matrix *copied_image = alloc_matrix(height, width, channels);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < channels; k++){
                copied_image->data[i][j][k] = image->data[i][j][k];
            }
        }
    }
    return copied_image;
}

void free_matrix(Matrix *image) {
    int height = image->height;
    int width = image->width;

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

Matrix *read_matrix(string filename, int height, int width, int channels) {
    Matrix *image = alloc_matrix(height, width, channels);
    FILE *file_ptr = fopen(filename, "rb");
    if (!file_ptr)
    {
        fprintf(stderr, "ERROR: read_matrix() \n\t File could not be opened\n");
        return NULL;    
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
    fclose(file_ptr);
    return image;
}

void write_matrix(string filename, Matrix *image) {
    int height = image->height;
    int width = image->width;
    int channels = image->channels;

    FILE *file_ptr = fopen(filename, "wb");
    if (!file_ptr)
    {
        fprintf(stderr, "ERROR: write_matrix() \n\t File could not be opened\n");
        return;    
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
    fclose(file_ptr);
    return;
}

BYTE to_BYTE(float value) {
    if (value >= MAX_INTENSITY) {
        return MAX_INTENSITY;
    }
    else if (value <= MIN_INTENSITY)
    {
       return MIN_INTENSITY;
    }
    else {
        return round(value);
    }
}