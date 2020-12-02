/**
 * (C) Copyright 2020 - Proiect APP
 *                      Blurarea imaginilor
 * Georgescu Alin-Andrei 342 C3
 * Iuga Florin-Eugen     342 C5
 * Negru Bogdan-Crisitan 342 C3
 */

#include <stdio.h>
#include <stdlib.h>

#include "../utils/blur_utils.h"
#include "../utils/blur_io.h"

int main(int argc, char *argv[]) {
    int width, height;
    int i;
    uint8_t type, channels, maxval;
    uint8_t **image;
    char *image_in;
    char *image_out;
    int num_filters;

    if (argc < 4) {
        fprintf(stderr, "Eroare!\nMod utilizare: "
                        "./blur <in.format> <out.format> <num_applied>\n");

        return 0;
    } else {
        image_in = argv[1];
        image_out = argv[2];
        num_filters = atoi(argv[3]);

        if (num_filters <= 0) {
            fprintf(stderr, "Eroare!\n<num_applied> trebuie sa fie strict "
                            "pozitiv\n");

            return 0;
        }
    }

    // Citirea imaginii de intrare din fișier.
    read_image(image_in, &type, &channels, &width, &height, &maxval, &image);

    // Aplicarea filtrului.
    for (i = 0; i < num_filters; ++i) {
        apply_filter(height, width, channels, maxval, image);
    }

    // Scrierea în fișier a imaginii.
    print_image(image_out, &type, &channels, &width, &height, &maxval, image);

    for (i = 0; i < height + 2; ++i) {
        free(image[i]);
    }
    free(image);

    return 0;
}
