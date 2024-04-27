#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iir.h"
#include "utils.h"

IIR *init_IIR(double* a, int M, double* b, int N){
    //initialize the IIR filter
    IIR* iir = (IIR*)malloc(sizeof(IIR));
    iir->xbuf = (data_t*)malloc(M*sizeof(data_t));
    iir->ybuf = (data_t*)malloc(N*sizeof(data_t));
    //check if memory allocation was successful
    if(iir == NULL || iir->xbuf == NULL || iir->ybuf == NULL){
        fprintf(stderr, "Error: Failed to allocate memory for IIR filter\n");
        free(iir->xbuf);
        free(iir->ybuf);
        free(iir);
        exit(EXIT_FAILURE);
    }
    //initialize the coefficients 
    iir->a = a;
    iir->M = M;
    iir->b = b;
    iir->N = N;
    
    return iir;
}

void reset_IIR(IIR* iir){
    //reset the internal buffers of the IIR filter
    memset(iir->xbuf, 0, iir->M * sizeof(data_t));
    memset(iir->ybuf, 0, iir->N * sizeof(data_t));
}

void filter_IIR(IIR* iir, data_t* x, data_t* y, int buffer_size){
    data_t x_temp[iir->M + buffer_size]; 
    data_t y_temp[iir->N + buffer_size];

    //append internal buffer and input/output to temporary buffers
    memcpy(x_temp, iir->xbuf, iir->M * sizeof(data_t));
    memcpy(x_temp + iir->M, x, buffer_size * sizeof(data_t));

    memcpy(y_temp, iir->ybuf, iir->N * sizeof(data_t));
    memcpy(y_temp + iir->N, y, buffer_size * sizeof(data_t));

    //filter the signal using difference equation of the filter
    for(int i = 0; i < buffer_size; i++){
        y_temp[i+iir->M] = 0;
        for(int j = i; j < i + iir->N; j++){
            y_temp[i+iir->M] += (data_t)(iir->b[j-i]*x_temp[j]);
        }
        for(int j = i; j < i + iir->M; j++){
            y_temp[i+iir->M] -= (data_t)(iir->a[j-i]*y_temp[j]);
        }
    }
    //write on output 
    memcpy(y, y_temp + iir->M, buffer_size * sizeof(data_t));

    //update internal buffers
    memcpy(iir->xbuf, x_temp + buffer_size, iir->M * sizeof(data_t));
    memcpy(iir->ybuf, y_temp + buffer_size, iir->N * sizeof(data_t));
}


void free_IIR(IIR* iir){
    //free the memory allocated for the IIR filter
    free(iir->a);
    free(iir->b);
    free(iir->xbuf);
    free(iir->ybuf);
    free(iir);
}