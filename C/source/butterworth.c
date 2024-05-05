
#include <stdlib.h>
#include <stdio.h>

#include "butterworth.h"
#include "utils.h"
#include "iir.h"

Butterworth *init_butterworth(int type, double fc){
    //initialize the butterworth filter
    printf("coucou\n");
    Butterworth* butterworth = (Butterworth*)malloc(sizeof(Butterworth));
    Coefficients *a = (Coefficients*)malloc(sizeof(Coefficients)); 
    Coefficients *b = (Coefficients*)malloc(sizeof(Coefficients));
    //check if memory allocation was successful
    if (butterworth == NULL || a == NULL || b == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for butterworth filter\n");
        free(butterworth);
        free(a);
        free(b);
        exit(EXIT_FAILURE);   
    }
    //compute the coefficients of the butterworth filter
    if(type == LOWPASS){
        //TODO compute the coefficients of the low-pass butterworth filter
    } else {
        //TODO compute the coefficients of the high-pass butterworth
    }
    butterworth->iir = init_IIR(a, b);
    return butterworth;
}

void reset_butterworth(Butterworth* butterworth){
    //reset the internal buffers of the butterworth filter
    reset_IIR(butterworth->iir);
}

void filter_butterworth(Butterworth* butterworth, data_t* x, data_t* y, int buffer_size){
    //filter the signal using the IIR filter
    filter_IIR(butterworth->iir, x, y, buffer_size);
}

void free_butterworth(Butterworth* butterworth){
    //free the memory allocated for the butterworth filter
    free_IIR(butterworth->iir);
    free(butterworth);
}

