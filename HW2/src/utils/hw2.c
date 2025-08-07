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

Matrix *image_padding(Matrix *image, Matrix *filter) {
    int height = image->height;
    int width = image->width;
    int channels = image->channels;
    int filter_height = filter->height;
    int filter_width = filter->width;

    int new_height = height + (filter_height - 1);
    int new_width = width + (filter_width - 1);

    int height_offset = (filter_height - 1) / 2;
    int width_offset = (filter_width - 1) / 2;
    Matrix *output_image = alloc_matrix(new_height, new_width, channels);
    if (!output_image) {
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < channels; k++) {
                output_image->data[i + height_offset][j + width_offset][k] = image->data[i][j][k];
            }
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width_offset; j++) {
            for (int k = 0; k < channels; k++) {
                output_image->data[i][width_offset - j][k] = output_image->data[i][width_offset + 1 + j][k];
                output_image->data[i][width_offset - j][k] = output_image->data[i][width_offset + 1 + j][k];
            }
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width_offset; j++) {
            for (int k = 0; k < channels; k++) {
                output_image->data[i][width_offset - j][k] = output_image->data[i][width_offset + 1 + j][k];
                output_image->data[i][width_offset - j][k] = output_image->data[i][width_offset + 1 + j][k];
            }
        }
    }

    return output_image;
}

Matrix *raster_convolution(Matrix *image, Matrix *filter) {
    int height = image->height;
    int width = image->width;
    int channels = image->channels;
    int filter_height = filter->height;
    int filter_width = filter->width;
    int filter_channels = filter->channels;
    int height_offset = (filter_height - 1) / 2;
    int width_offset = (filter_width - 1) / 2;

    Matrix *padded_image = image_padding(image, filter);
    if (!padded_image) {
        return NULL;
    }

    Matrix *output_image = alloc_matrix(height, width, channels);
    if (!output_image) {
        free_matrix(padded_image);
        return NULL;
    }

    float window_sum;

    for (int k = 0; k < channels; k++) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                window_sum = 0;
                for (int a = -height_offset; a <= height_offset; a++) {
                    for (int b = -width_offset; b <= width_offset; b++) {
                        window_sum += filter->data[a + height_offset][b + width_offset][1] * padded_image->data[i + height_offset + a][j + width_offset + b][k];
                    }
                }
                output_image->data[i][j][k] = window_sum;
            }
        }
    }

    free_matrix(padded_image);
    return output_image;
}

Matrix *serpentine_convolution(Matrix *image, Matrix *filter) {
    int height = image->height;
    int width = image->width;
    int channels = image->channels;
    int filter_height = filter->height;
    int filter_width = filter->width;
    int filter_channels = filter->channels;
    int height_offset = (filter_height - 1) / 2;
    int width_offset = (filter_width - 1) / 2;

    Matrix *padded_image = image_padding(image, filter);
    if (!padded_image) {
        return NULL;
    }

    Matrix *output_image = alloc_matrix(height, width, channels);
    if (!output_image) {
        free_matrix(padded_image);
        return NULL;
    }

    float window_sum;

    for (int k = 0; k < channels; k++) {
        for (int i = 0; i < height; i++) {
            if ((i % 2) == 0) {
                for (int j = 0; j < width; j++) {
                    window_sum = 0;
                    for (int a = -height_offset; a <= height_offset; a++) {
                        for (int b = -width_offset; b <= width_offset; b++) {
                            window_sum += filter->data[a + height_offset][b + width_offset][1] * padded_image->data[i + height_offset + a][j + width_offset + b][k];
                        }
                    }
                    output_image->data[i][j][k] = window_sum;
                }
            }
            else {
                for (int j = width - 1; j >= 0; j--) {
                    window_sum = 0;
                    for (int a = -height_offset; a <= height_offset; a++) {
                        for (int b = -width_offset; b <= width_offset; b++) {
                            window_sum += filter->data[a + height_offset][b + width_offset][1] * padded_image->data[i + height_offset + a][j + width_offset + b][k];
                        }
                    }
                    output_image->data[i][j][k] = window_sum;
                }
            }
        }
    }

    free_matrix(padded_image);
    return output_image;
}

Matrix *serpentine_error_diffusion(Matrix *image, Matrix *filter, BYTE threshold) {
    int height = image->height;
    int width = image->width;
    int channels = image->channels;
    int filter_height = filter->height;
    int filter_width = filter->width;
    int filter_channels = filter->channels;
    int height_offset = (filter_height - 1) / 2;
    int width_offset = (filter_width - 1) / 2;

    Matrix *padded_image = image_padding(image, filter);
    if (!padded_image) {
        return NULL;
    }

    Matrix *output_image = alloc_matrix(height, width, channels);
    if (!output_image) {
        free_matrix(padded_image);
        return NULL;
    }

    Matrix *filter_flipped = alloc_matrix(filter_height, filter_width, filter_channels);
    if (!filter_flipped) {
        free_matrix(padded_image);
        free_matrix(output_image);
        return NULL;
    }

    for (int i = 0; i < filter_height; i++) {
        for (int j = 0; j < filter_width; j++) {
            for (int k = 0; k < filter_channels; k++) {
                filter_flipped->data[i][j][k] = filter->data[i][filter_width - 1 - j][k];
            }
        }
    }
    
    float error;
    for (int k = 0; k < channels; k++) {
        for (int i = 0; i < height; i++) {
            if ((i % 2) == 0) {
                for (int j = 0; j < width; j++) {
                    output_image->data[i][j][k] = (padded_image->data[i + height_offset][j + width_offset][k] > threshold) ? MAX_INTENSITY : MIN_INTENSITY;
                    error = padded_image->data[i + height_offset][j + width_offset][k] - output_image->data[i][j][k];
                    for (int a = -height_offset; a <= height_offset; a++) {
                        for (int b = -width_offset; b <= width_offset; b++) {
                            padded_image->data[i + a][j + b][k] += filter->data[a + height_offset][b + width_offset][1] * error;
                        }
                    }
                }
            }
            else {
                for (int j = width - 1; j >= 0; j--) {
                    output_image->data[i][j][k] = (padded_image->data[i + height_offset][j + width_offset][k] > threshold) ? MAX_INTENSITY : MIN_INTENSITY;
                    error = padded_image->data[i + height_offset][j + width_offset][k] - output_image->data[i][j][k];
                    for (int a = -height_offset; a <= height_offset; a++) {
                        for (int b = -width_offset; b <= width_offset; b++) {
                            padded_image->data[i + a][j + b][k] += filter_flipped->data[a + height_offset][b + width_offset][1] * error;
                        }
                    }
                }
            }
        }
    }

    free_matrix(padded_image);
    return output_image;
}

Matrix *RGB_to_CMY(Matrix *image) {
    int height = image->height;
    int width = image->width;
    int channels = image->channels;

    Matrix *output = alloc_matrix(height, width, channels);
    if (!output) {
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < channels; k++) {
                output->data[i][j][k] = 1 - (image->data[i][j][k] / (float) MAX_INTENSITY);
            }
        }
    }
    return output;
}

Matrix *CMY_to_RGB(Matrix *image) {
    int height = image->height;
    int width = image->width;
    int channels = image->channels;

    Matrix *output = alloc_matrix(height, width, channels);
    if (!output) {
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < channels; k++) {
                output->data[i][j][k] = 1 - (image->data[i][j][k] / (float) MAX_INTENSITY);
            }
        }
    }
    return output;
}