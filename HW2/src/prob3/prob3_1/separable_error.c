#include "../../utils/hw2.h"

int main(int argc, char **argv) {
    if (argc != 5) {
        fprintf(stderr, "USAGE: ./separable_error filename height width channels");
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];
    int height = atoi(argv[2]);
    int width = atoi(argv[3]);
    int channels = atoi(argv[4]);

    char out_filename[100];
    sprintf(out_filename, "../data/output/separable_error.raw");

    Matrix *image = read_matrix(filename, height, width, channels);
    if (!image) {
        exit(EXIT_FAILURE);
    }

    Matrix *separable_error_diff_matrix = alloc_matrix(3, 3, 1);
    if(!separable_error_diff_matrix) {
        free_matrix(image);
        exit(EXIT_FAILURE);
    }
    separable_error_diff_matrix->data[0][0][1] = 0, separable_error_diff_matrix->data[0][1][1] = 0, separable_error_diff_matrix->data[0][2][1] = 0;
    separable_error_diff_matrix->data[1][0][1] = 0, separable_error_diff_matrix->data[1][1][1] = 0, separable_error_diff_matrix->data[1][2][1] = 7.0 / 16;
    separable_error_diff_matrix->data[2][0][1] = 3.0 / 16, separable_error_diff_matrix->data[2][1][1] = 5.0 / 16, separable_error_diff_matrix->data[2][2][1] = 1.0 / 16;
    
    Matrix *output = serpentine_convolution(image, separable_error_diff_matrix);
    if (!output) {
        free_matrix(image);
        free_matrix(separable_error_diff_matrix);
        exit(EXIT_FAILURE);
    }

    write_matrix(out_filename, output);
    free_matrix(image);
    free_matrix(separable_error_diff_matrix);
    free_matrix(output);
    exit(EXIT_SUCCESS);
}

