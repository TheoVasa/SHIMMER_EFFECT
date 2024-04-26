
#include <stdlib.h>

#include "butterworth.h"
#include "utils.h"
#include "iir.h"

Butterworth *init_butterworth(int type, double fc){
    //initialize the butterworth filter
    Butterworth* butterworth = (Butterworth*)malloc(sizeof(Butterworth));
    double *a = (double*)malloc(ORDER*sizeof(double));
    double *b = (double*)malloc(ORDER*sizeof(double));  
    //compute the coefficients of the butterworth filter
    if(type == LOWPASS){
        //TODO compute the coefficients of the low-pass butterworth filter
    } else {
        //TODO compute the coefficients of the high-pass butterworth
    }
    butterworth->iir = init_IIR(a, ORDER, b, ORDER);
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

