#include "../../utils/hw2.h"

int main(int argc, char **argv) {
    if (argc != 5) {
        fprintf(stderr, "USAGE: ./stucki filename height width channels");
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];
    int height = atoi(argv[2]);
    int width = atoi(argv[3]);
    int channels = atoi(argv[4]);

    char out_filename[100];
    sprintf(out_filename, "../data/output/stucki.raw");

    Matrix *image = read_matrix(filename, height, width, channels);
    if (!image) {
        exit(EXIT_FAILURE);
    }

    Matrix *stucki_diff_matrix = alloc_matrix(5, 5, 1);
    if(!stucki_diff_matrix) {
        free_matrix(image);
        exit(EXIT_FAILURE);
    }
    stucki_diff_matrix->data[0][0][1] = 0, stucki_diff_matrix->data[0][1][1] = 0, stucki_diff_matrix->data[0][2][1] = 0, stucki_diff_matrix->data[0][3][1] = 0, stucki_diff_matrix->data[0][4][1] = 0;
    stucki_diff_matrix->data[1][0][1] = 0, stucki_diff_matrix->data[1][1][1] = 0, stucki_diff_matrix->data[1][2][1] = 0,  stucki_diff_matrix->data[1][3][1] = 0, stucki_diff_matrix->data[1][4][1] = 0;
    stucki_diff_matrix->data[2][0][1] = 0, stucki_diff_matrix->data[2][1][1] = 0, stucki_diff_matrix->data[2][2][1] = 0,  stucki_diff_matrix->data[2][3][1] = 7.0 / 42, stucki_diff_matrix->data[2][4][1] = 5.0 / 42;
    stucki_diff_matrix->data[3][0][1] = 3.0 / 42, stucki_diff_matrix->data[3][1][1] = 5.0 / 42, stucki_diff_matrix->data[3][2][1] = 7.0 / 42,  stucki_diff_matrix->data[3][3][1] = 5.0 / 42, stucki_diff_matrix->data[3][4][1] = 3.0 / 42;
    stucki_diff_matrix->data[4][0][1] = 1.0 / 42, stucki_diff_matrix->data[4][1][1] = 3.0 / 42, stucki_diff_matrix->data[4][2][1] = 5.0 / 42,  stucki_diff_matrix->data[4][3][1] = 3.0 / 42, stucki_diff_matrix->data[4][4][1] = 1.0 / 42;

    Matrix *output = serpentine_convolution(image, stucki_diff_matrix);
    if (!output) {
        free_matrix(image);
        free_matrix(stucki_diff_matrix);
        exit(EXIT_FAILURE);
    }

    write_matrix(out_filename, output);
    free_matrix(image);
    free_matrix(stucki_diff_matrix);
    free_matrix(output);
    exit(EXIT_SUCCESS);
}

