#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <pthread.h>
#include "../utils/blur_utils.h"
#include "../utils/blur_io.h"


pthread_barrier_t barrier;

int NUM_FILTERS = 1;
int num_th = 0;

struct image {
    uint8_t **image;
    uint8_t type, channels, maxval;
    int width;
    int height;
    int id;
    int num_tasks;
};

int minim(int a, int b) {
	if (a < b) {
		return a;
	}
	return b;
}

void apply_filter_pthreads(int height, int width, uint8_t channels, uint8_t maxval, int start, int end,
                            uint8_t **image) {
    int i, j;
    float pixel;
    uint8_t **aux;

     // se aloca emorie locala pentru a evita race conditions pe liniile de pixeli comune
    
    aux = (uint8_t **) malloc((height) * sizeof(uint8_t *));
    check_container(aux, NULL);
    
    int k = 0;
    for (i = start; i < end; ++i) {
        aux[k] = (uint8_t *) malloc(channels * (width + 2) * sizeof(uint8_t));
        check_container(aux[k], NULL);
        memcpy(aux[k], image[i], channels * (width + 2) * sizeof(uint8_t));
        k++;
    }

    // se asteapta pana termina toate thread-urile de copiat memoria in local memory,
    // pentru a evita situatia cand un thread ar copia pixeli cu filtru deja aplicat
    pthread_barrier_wait(&barrier);

    // Aplicarea filtrului propriu-zis (a convoluției)
    for (i = 1; i < height - 1; ++i) {
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

            image[start + i][j] = pixel;
        }
    }

    for (i = 0; i < height; ++i) {
        free(aux[i]);
    }
    free(aux);
}

void *thread_task(void *arg)
{
    struct image img = *(struct image*) arg;
    int start, end;

    start = img.id * ceil((double) img.height / img.num_tasks);
    end = minim(img.height, (img.id + 1) * ceil((double) img.height / img.num_tasks));

    if (img.id > 0) {
        start--;
    }

    end++;
    
    if (img.id == img.num_tasks - 1) {
        end++;
    }

    img.height = end - start;

    for (int t = 0; t < NUM_FILTERS; t++) {
        // make sure that all threads had finished the job for the previous filter
        // before going to the next one
        pthread_barrier_wait(&barrier);
        apply_filter_pthreads(img.height, img.width, img.channels, img.maxval, start, end, img.image);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int width, height;
    uint8_t type, channels, maxval;
    uint8_t **image;
    char *image_in = argv[1];
    char *image_out = argv[2];
    int NUM_THREADS = atoi(argv[3]);
    num_th = NUM_THREADS;
    NUM_FILTERS = atoi(argv[4]);

    if (argc < 5) {
        fprintf(stderr, "Eroare! Numar de argumente invalid! Insereaza: in out num_threads num_blurs\n");
        return 0;
    }

    // Citirea imaginii de intrare din fișier.
    read_image(image_in, &type, &channels, &width, &height,
                &maxval, &image);

    pthread_t threads[NUM_THREADS];
    struct image img[NUM_THREADS]; 
    int r;
    int id;


    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    for (id = 0; id < NUM_THREADS; id++) {
        img[id].image = image;
        img[id].channels = channels;
        img[id].height = height;
        img[id].width = width;
        img[id].id = id;
        img[id].maxval = maxval;
        img[id].type = type;
        img[id].num_tasks = NUM_THREADS;

        r = pthread_create(&threads[id], NULL, thread_task, (void *) &img[id]);
 
        if (r) {
            printf("Eroare la crearea thread-ului %d\n", id);
            exit(-1);
        }

    }

    void *status;
     for (id = 0; id < NUM_THREADS; id++) {
        r = pthread_join(threads[id], &status);
 
        if (r) {
            printf("Eroare la asteptarea thread-ului %d\n", id);
            exit(-1);
        }
    }


    // Scrierea în fișier a imaginii.
    print_image(image_out, &type, &channels,
                &width, &height, &maxval, image);

}