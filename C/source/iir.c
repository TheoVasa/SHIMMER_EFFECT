#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iir.h"
#include "utils.h"

IIR *init_IIR(double* a, int M, double* b, int N){
    //initialize the IIR filter
    IIR* iir = (IIR*)malloc(sizeof(IIR));
    iir->xbuf = (data_t*)malloc((N+MAX_BUFFER_SIZE)*sizeof(data_t));
    iir->ybuf = (data_t*)malloc((M+MAX_BUFFER_SIZE)*sizeof(data_t));
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
    memset(iir->xbuf, 0, (iir->N+MAX_BUFFER_SIZE) * sizeof(data_t));
    memset(iir->ybuf, 0, (iir->M+MAX_BUFFER_SIZE) * sizeof(data_t));
}

void filter_IIR(IIR* iir, data_t* x, data_t* y, int buffer_size){
    //append input to internal buffer
    memcpy(iir->xbuf+iir->N, x, buffer_size * sizeof(data_t));
    //append zeros to output buffer
    memset(iir->ybuf+iir->M, 0, buffer_size * sizeof(data_t)); 

    //filter the signal using difference equation of the filter
    for(int i = 0; i < buffer_size; i++){
        y[i] = 0; 
        for(int j = 0; j < iir->N; j++){
            y[i] += (data_t)(iir->b[j] * (double)iir->xbuf[i + j]);
        }
        for(int j = 0; j < iir->M; j++){
            y[i] -= (data_t)(iir->a[j] * (double)iir->ybuf[i + j]);
        }
    }

    //update internal buffers
    if(iir->N > 0){
    memmove(iir->xbuf, iir->xbuf + buffer_size, iir->N * sizeof(data_t));
    } 
    memmove(iir->ybuf, iir->ybuf + buffer_size, iir->M * sizeof(data_t));
}


void free_IIR(IIR* iir){
    //free the memory allocated for the IIR filter
    free(iir->a);
    free(iir->b);
    free(iir->xbuf);
    free(iir->ybuf);
    free(iir);
}