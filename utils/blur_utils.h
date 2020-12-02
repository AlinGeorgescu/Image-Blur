/**
 * (C) Copyright 2020 - Proiect APP
 *                      Blurarea imaginilor
 * Georgescu Alin-Andrei 342 C3
 * Iuga Florin-Eugen     342 C5
 * Negru Bogdan-Crisitan 342 C3
 */

#ifndef BLUR_UTILS_H_
#define BLUR_UTILS_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef MPI_VERSION
#include "mpi.h"
#endif  // MPI_VERSION

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

// Matricea filtrului Blur Gaussian
static const float GAUSSIAN[3][3] = {
    {1.f / 16, 2.f / 16, 1.f / 16},
    {2.f / 16, 4.f / 16, 2.f / 16},
    {1.f / 16, 2.f / 16, 1.f / 16}
};

/**
 * Verifică dacă fișierul s-a deschis cu succes sau nu.
 *
 * @param file = descriptorul de fișier
 */
void check_file(FILE *file) {
    if (!file) {
        fprintf(stderr, "Eroare la deschiderea fișierelor!\n");

        #ifdef MPI_VERSION
        MPI_Finalize();
        #endif  // MPI_VERSION

        exit(-1);
    }
}

/**
 * Verifică dacă s-a putut aloca memorie pentru structura de date sau nu.
 *
 * @param ptr  = pointerul către container
 * @param file = fișierul ce trebuie înschis înainte de închiderea programului
 */
void check_container(void *ptr, FILE *file) {
    if (!ptr) {
        fprintf(stderr, "Eroare la alocarea memoriei!\n");
        if (file) {
            fclose(file);
        }

        #ifdef MPI_VERSION
        MPI_Finalize();
        #endif  // MPI_VERSION

        exit(-1);
    }
}

/**
 * Aplică filtrul pe o imagine / bucată de imagine.
 *
 * @param height   = înălțimea fragmentului de imagine editat
 * @param width    = lățimea fragmentului de imagine editat
 * @param channels = numărul de canale al imaginii (color / alb-negru)
 * @param maxval   = valoarea maximă a unui pixel
 * @param image    = matricea imaginii
 */
void apply_filter(int height, int width, uint8_t channels, uint8_t maxval,
                  uint8_t **image) {
    int i, j;
    float pixel;
    uint8_t **aux;

    aux = (uint8_t **) malloc((height + 2) * sizeof(uint8_t *));
    check_container(aux, NULL);

    for (i = 0; i < height + 2; ++i) {
        aux[i] = (uint8_t *) malloc(channels * (width + 2) * sizeof(uint8_t));
        check_container(aux[i], NULL);
        memcpy(aux[i], image[i], channels * (width + 2) * sizeof(uint8_t));
    }

    // Aplicarea filtrului propriu-zis (a convoluției)
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

#endif  // BLUR_UTILS_H_
