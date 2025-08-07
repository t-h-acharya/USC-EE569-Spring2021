#include "../../utils/hw2.h"
#include <time.h>

int main(int argc, char **argv) {
    if (argc != 5) {
        fprintf(stderr, "USAGE: ./random_thresholding filename height width channels");
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];
    int height = atoi(argv[2]);
    int width = atoi(argv[3]);
    int channels = atoi(argv[4]);

    char out_filename[100];
    sprintf(out_filename, "../data/output/random_thresholding.raw");

    Matrix *image = read_matrix(filename, height, width, channels);
    if (!image) {
        exit(EXIT_FAILURE);
    }
    
    srand(time(NULL));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < channels; k++) {
                image->data[i][j][k] = (image->data[i][j][k] < (rand() % MAX_INTENSITY)) ? MIN_INTENSITY : MAX_INTENSITY;
            }
        }
    }

    write_matrix(out_filename, image);
    
    free_matrix(image);

    exit(EXIT_SUCCESS);
}
