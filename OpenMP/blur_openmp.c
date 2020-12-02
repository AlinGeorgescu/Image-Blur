#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#include "../utils/blur_utils.h"
#include "../utils/blur_io.h"

int minim(int a, int b) {

	if (a < b) {
		return a;
	}
	return b;
}

void apply_filter_mp(uint8_t **image, int height, int width, uint8_t maxval, uint8_t channels) {

    int i, j;
    float pixel;
    uint8_t **aux;
    aux = (uint8_t **) malloc((height + 2) * sizeof(uint8_t *));
    check_container(aux, NULL);

    #pragma omp for schedule(static, 4)
    for (i = 0; i < height + 2; ++i) {
        aux[i] = (uint8_t *) malloc(channels * (width + 2) * sizeof(uint8_t));
        check_container(aux[i], NULL);
        memcpy(aux[i], image[i], channels * (width + 2) * sizeof(uint8_t));
    }

    #pragma omp for schedule(static, 4)
    for (i = 1; i < height + 1; ++i) {
        for (j = channels; j < channels * (width + 1); ++j) {
            pixel = GAUSSIAN[2][2] * aux[i - 1][j - channels] +
                    GAUSSIAN[2][1] * aux[i - 1][j]            +
                    GAUSSIAN[2][0] * aux[i - 1][j + channels] +
                    GAUSSIAN[1][2] * aux[i][j - channels]     +
                    GAUSSIAN[1][1] * aux[i][j]                +
                    GAUSSIAN[1][0] * aux[i][j + channels]     +
                    GAUSSIAN[0][2] * aux[i + 1][j - channels] +
                    GAUSSIAN[0][1] * aux[i + 1][j]            +
                    GAUSSIAN[0][0] * aux[i + 1][j + channels];
            pixel = MIN(maxval, pixel);
            pixel = MAX(0, pixel);
            image[i][j] = pixel;
        }
    }
    for (i = 0; i < height + 2; ++i) {
        free(aux[i]);
    }
    free(aux);
}

int main(int argc, char *argv[]) {

    int width, height;
    uint8_t type, channels, maxval;
    uint8_t **image;
    char *image_in = argv[1];
    char *image_out = argv[2];
    int NUM_FILTERS = atoi(argv[3]);

    if (argc < 4) {
        fprintf(stderr, "Eroare! Numar de argumente invalid! Insereaza: in out num_threads num_blurs\n");
        return 0;
    }

    // Citirea imaginii de intrare din fișier.
    read_image(image_in, &type, &channels, &width, &height,
                &maxval, &image);

    for (int i = 0; i < NUM_FILTERS; ++i) {
        apply_filter_mp(image, height, width, maxval, channels);
    }

    // Scrierea în fișier a imaginii.
    print_image(image_out, &type, &channels,
                &width, &height, &maxval, image);

    for (int i = 0; i < height + 2; ++i) {
        free(image[i]);
    }
    free(image);

}