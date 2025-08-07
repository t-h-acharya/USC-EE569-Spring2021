#include "../../utils/hw2.h"

int **get_dithering_matrix(int N);

int main(int argc, char **argv) {
    if (argc != 6) {
        fprintf(stderr, "USAGE: ./fixed_thresholding filename height width channels index_matrix_size");
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];
    int height = atoi(argv[2]);
    int width = atoi(argv[3]);
    int channels = atoi(argv[4]);
    if (atoi(argv[5]) <= 0) {
        fprintf(stderr, "ERROR: Invalid index_matrix_size\n");
        exit(EXIT_FAILURE);
    }
    int N = (int) pow(2, atoi(argv[5]));

    char out_filename[100];
    sprintf(out_filename, "../data/output/dithering_matrix_%d.raw", N);

    Image *image = read_image(filename, height, width, channels);
    if (!image) {
        exit(EXIT_FAILURE);
    }

    int **dithering_matrix = get_dithering_matrix(N);
    if(!dithering_matrix) {
        free_image(image);
        exit(EXIT_FAILURE);
    }

    float **thresholding_matrix = (float **)malloc(N * sizeof(float *));
    if (!thresholding_matrix) {
        fprintf(stderr, "ERROR: main() \n\t Memory could not be allocated for thresholding matrix\n");
        free(thresholding_matrix);
        free_image(image);
    }

    for (int i = 0; i < N; i++) {
        thresholding_matrix[i] = (float *)malloc(N * sizeof(float));
        if (!thresholding_matrix[i]) {
            fprintf(stderr, "ERROR: main() \n\t Memory could not be allocated for thresholding matrix\n");
            for (int j = 0; j < i; j++) {
                free(thresholding_matrix[j]);
            }
            free(thresholding_matrix);
            free_image(image);
            exit(EXIT_FAILURE);
        }
    }

    for (int a = 0; a < N; a++) {
        for (int b = 0; b < N; b++) {
            thresholding_matrix[a][b] = (dithering_matrix[a][b] + 0.5) * MAX_INTENSITY / pow(N, 2);
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < channels; k++) {
                image->data[i][j][k] = (image->data[i][j][k] < (thresholding_matrix[i % N][j % N])) ? MIN_INTENSITY : MAX_INTENSITY;
            }
        }
    }

    write_image(out_filename, image);

    for (int i = 0; i < N; i++) {
        free(dithering_matrix[i]);
        free(thresholding_matrix[i]);
    }
    free(dithering_matrix);
    free(thresholding_matrix);
    free_image(image);
    exit(EXIT_SUCCESS);
}

int **get_dithering_matrix(int N) {
    if (N == 2) {
        int** dithering_matrix = (int **)malloc(N * sizeof(int *));
        if (!dithering_matrix) {
            fprintf(stderr, "ERROR: get_dithering_matrix() \n\t Memory could not be allocated.");
            return NULL;
        }
        for (int i = 0; i < N; i++) {
            dithering_matrix[i] = (int *)malloc(N * sizeof(int));
            if (!dithering_matrix[i]) {
                fprintf(stderr, "ERROR: get_dithering_matrix() \n\t Memory could not be allocated.");
                for (int j = 0; j < i; j++) {
                    free(dithering_matrix[j]);
                }
                free(dithering_matrix);
                return NULL;
            }
        }

        dithering_matrix[0][0] = 1;
        dithering_matrix[0][1] = 2;
        dithering_matrix[1][0] = 3;
        dithering_matrix[1][1] = 0;
        return dithering_matrix;
    }

    int **dithering_matrix_previous = get_dithering_matrix(N / 2);
    int **dithering_matrix = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        dithering_matrix[i] = (int *)malloc(N * sizeof(int));
    }

    for (int i = 0; i < N / 2; i++) {
        for (int j = 0; j < N /2; j++) {
            dithering_matrix[i][j] = dithering_matrix_previous[i][j] * 4 + 1;
            dithering_matrix[i][j + N / 2] = dithering_matrix_previous[i][j] * 4 + 2;
            dithering_matrix[i + N / 2][j] = dithering_matrix_previous[i][j] * 4 + 3;
            dithering_matrix[i + N / 2][j + N / 2] = dithering_matrix_previous[i][j] * 4;
        }
    }

    for (int i = 0; i < N / 2; i++) {
        free(dithering_matrix_previous[i]);
    }
    free(dithering_matrix_previous);
    return dithering_matrix;
}