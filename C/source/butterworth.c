
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "butterworth.h"
#include "utils.h"
#include "iir.h"
#include "iir.h"
#include "filterlib.h"

Butterworth *init_butterworth(int type, double fc){
    //create a digital butterworth filter using the library https://github.com/adis300/filter-c
    Butterworth *butterworth = (Butterworth*)malloc(sizeof(Butterworth));
    butterworth->type = (int*)malloc(sizeof(int));
    butterworth->fc = (double*)malloc(sizeof(double));
    //check if memory allocation was successful
    if(butterworth == NULL || butterworth->type == NULL || butterworth->fc == NULL){
        fprintf(stderr, "Error: Failed to allocate memory for butterworth filter\n");
        free(butterworth->type);
        free(butterworth->fc);
        free(butterworth);
        exit(EXIT_FAILURE);
    }
    //create the butterworth filter
    ButterCoeff *butter;
    if(type == LOWPASS){
        *butterworth->type = LOWPASS;
        butter = create_bw_low_pass_filter(ORDER, SAMPLE_RATE, fc);
    } else {
        *butterworth->type = HIGHPASS;
        butter = create_bw_high_pass_filter(ORDER, SAMPLE_RATE, fc);
    }
    
    butterworth->butter = butter;
    *butterworth->fc = fc;
    return butterworth;
}

void reset_butterworth(Butterworth* butterworth){
    //the external library doesn't offer a reset function, we will have to recreate one
    if(*butterworth->type == LOWPASS){
        free_bw_low_pass(butterworth->butter);
        butterworth->butter = create_bw_low_pass_filter(ORDER, SAMPLE_RATE, *butterworth->fc);
    } else {
        free_bw_high_pass(butterworth->butter);
        butterworth->butter = create_bw_high_pass_filter(ORDER, SAMPLE_RATE, *butterworth->fc);
    }
}

void filter_butterworth(Butterworth* butterworth, data_t* x, data_t* y, int buffer_size){
    if(*butterworth->type == HIGHPASS){
        //filter the signal using the butterworth filter (high-pass)
        for(int i=0; i<buffer_size; i++){
            y[i] = bw_high_pass(butterworth->butter, x[i]);
        }
    } else {
        //filter the signal using the butterworth filter (low-pass)
        for(int i=0; i<buffer_size; i++){
            y[i] = bw_low_pass(butterworth->butter, x[i]);
        }   
     }
}

void free_butterworth(Butterworth* butterworth){
    //free the memory allocated for the butterworth filter
    if(*butterworth->type == LOWPASS){
        free_bw_low_pass(butterworth->butter);
    } else {
        free_bw_high_pass(butterworth->butter);
    }
    //free the struct
    free(butterworth->type);
    free(butterworth->fc);
    free(butterworth);
}

