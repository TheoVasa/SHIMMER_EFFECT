#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "utils.h"
#include "lfo.h"

LFO *init_lfo(double freq, double amp){
    //allocate memory
    LFO *lfo = (LFO*)malloc(sizeof(LFO));
    lfo->amp = (double*)malloc(sizeof(double));
    lfo->freq = (double*)malloc(sizeof(double));
    lfo->start_time = (float*)malloc(sizeof(float));
    lfo->running = (int*)malloc(sizeof(int));
    //test the allocation
    if(lfo == NULL || lfo->amp == NULL || lfo->freq == NULL || lfo->start_time == NULL || lfo->running == NULL){
        fprintf(stderr, "Error: Failed to allocate memory for LFO\n");
        free(lfo->amp);
        free(lfo->freq);
        free(lfo->start_time);
        free(lfo->running);
        free(lfo);
        exit(EXIT_FAILURE);
    }
    *lfo->amp = amp;
    *lfo->freq = freq;
    *lfo->start_time = 0;
    *lfo->running = 0;
    return lfo;
}

void start_lfo(LFO* lfo){
    *lfo->start_time = clock();
    *lfo->running = 1;
}

void stop_lfo(LFO* lfo){
    *lfo->running = 0;
}

data_t lfo_get_value(LFO* lfo){
    if(*lfo->running){
        float time = (clock() - *lfo->start_time);
        return *lfo->amp* sin((double)(2 * M_PI * (*lfo->freq) * time /1000));
    }
    return 0;

}

void free_lfo(LFO* lfo){
    free(lfo->amp);
    free(lfo->freq);
    free(lfo->start_time);
    free(lfo->running);
    free(lfo);
}

