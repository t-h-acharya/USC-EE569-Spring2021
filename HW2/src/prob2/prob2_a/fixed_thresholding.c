#include "../../utils/hw2.h"

int main(int argc, char **argv) {
    if (argc != 6) {
        fprintf(stderr, "USAGE: ./fixed_thresholding filename height width channels threshold");
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];
    int height = atoi(argv[2]);
    int width = atoi(argv[3]);
    int channels = atoi(argv[4]);
    int threshold = atoi(argv[5]);
    char out_filename[100];
    sprintf(out_filename, "./data/output/fixed_thresholding_%d.raw", threshold);

    Image *image = read_image(filename, height, width, channels);
    if (!image) {
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < channels; k++) {
                image->data[i][j][k] = (image->data[i][j][k] < threshold) ? MIN_INTENSITY : MAX_INTENSITY;
            }
        }
    }

    write_image(out_filename, image);
    
    free_image(image);
    
    exit(EXIT_SUCCESS);
}
