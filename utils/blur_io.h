/**
 * (C) Copyright 2020 - Proiect APP
 *                      Blurarea imaginilor
 * Georgescu Alin-Andrei 342 C3
 * Iuga Florin-Eugen     342 C5
 * Negru Bogdan-Crisitan 342 C3
 */

#ifndef BLUR_IO_H_
#define BLUR_IO_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "blur_utils.h"

#define MAX_LEN  255

/**
 * Funcția de citire din fișier a unei imagini.
 *
 * @param image_in = denumirea fișierului
 * @param type     = adresa tipului imaginii (PNM / PGM)
 * @param channels = adresa numărului de canale al imaginii
 * @param width    = adresa lățimii imaginii
 * @param height   = adresa înălțimii imaginii
 * @param maxval   = adresa valorii maxime a unui pixel
 * @param image    = adresa matricei imaginii
 */
void read_image(char *image_in,
                uint8_t *type, uint8_t *channels,
                int *width, int *height,
                uint8_t *maxval, uint8_t ***image) {
    FILE *in;
    char comment_line[MAX_LEN];
    char c;
    int i;

    in = fopen(image_in, "rb");
    check_file(in);

    fscanf(in, "%c %hhu", &c, type);
    fgets(comment_line, MAX_LEN, in);
    fgets(comment_line, MAX_LEN, in);
    fscanf(in, "%d %d %hhu", width, height, maxval);
    fgets(comment_line, MAX_LEN, in);

    if ((*type) == 5) {
        *channels = 1;
    } else {
        *channels = 3;
    }

    *image = (uint8_t **) malloc(((*height) + 2) * sizeof(uint8_t *));
    check_container(*image, in);

    for (i = 0; i < (*height) + 2; ++i) {
        (*image)[i] =
            (uint8_t *) calloc((*channels) * ((*width) + 2), sizeof(uint8_t));
        check_container((*image)[i], in);
    }

    for (i = 1; i < (*height) + 1; ++i) {
        fread((*image)[i] + (*channels), sizeof(uint8_t),
              (*width) * (*channels), in);
    }

    fclose(in);
}

/**
 * Funcția de scriere în fișier a unei imagini.
 *
 * @param image_out = denumirea fișierului
 * @param type      = adresa tipului imaginii (PNM / PGM)
 * @param channels  = adresa numărului de canale al imaginii
 * @param width     = adresa lățimii imaginii
 * @param height    = adresa înălțimii imaginii
 * @param maxval    = adresa valorii maxime a unui pixel
 * @param image     = matricea imaginii
 */
void print_image(char *image_out,
                uint8_t *type, uint8_t *channels,
                int *width, int *height,
                uint8_t *maxval, uint8_t **image) {
    FILE *out;
    int i;

    out = fopen(image_out, "wb");
    check_file(out);

    fprintf(out, "P%u\n# Created by me\n", *type);
    fprintf(out, "%d %d\n", *width, *height);
    fprintf(out, "%d\n", *maxval);

    for (i = 1; i < (*height) + 1; ++i) {
        fwrite(image[i] + (*channels), sizeof(uint8_t),
               (*width) * (*channels), out);
    }

    fclose(out);
}

#endif  // BLUR_IO_H_
