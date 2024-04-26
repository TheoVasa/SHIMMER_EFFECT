#include "iir.h"
#include "utils.h"
#include <stdlib.h>

IIR *init_IIR(double* a, int M, double* b, int N){
    //initialize the IIR filter
    IIR* iir = (IIR*)malloc(sizeof(IIR));
    iir->a = a;
    iir->M = M;
    iir->b = b;
    iir->N = N;
    iir->xbuf = (double*)malloc(M*sizeof(double));
    iir->ybuf = (double*)malloc(N*sizeof(double));
    return iir;
}

void reset_IIR(IIR* iir){
    //reset the internal buffers of the IIR filter
    for(int i = 0; i < iir->M; i++){
        iir->xbuf[i] = 0;
    }
    for(int i = 0; i < iir->N; i++){
        iir->ybuf[i] = 0;
    }
}

void filter_IIR(IIR* iir, double* x, double* y, int buffer_size){
    //TODO optimize ! 
    double x_temp[iir->M + buffer_size]; 
    double y_temp[iir->N + buffer_size];

    //append internal buffer and input/output to temporary buffers
    for(int i = 0 ; i < iir->M + buffer_size; i++){
        if(i < iir->M){
            x_temp[i] = iir->xbuf[i];
        } else {
            x_temp[i] = x[i-iir->M];
        }
    }
    for(int i = 0 ; i < iir->N + buffer_size; i++){
        if(i < iir->N){
            y_temp[i] = iir->ybuf[i];
        } else {
            y_temp[i] = y[i-iir->N];
        }
    }
    //filter the signal using difference equation of the filter
    for(int i = 0; i < buffer_size; i++){
        y_temp[i+iir->M] = 0;
        for(int j = i; j < i + iir->N; j++){
            y_temp[i+iir->M] += iir->b[j-i]*x_temp[j];
        }
        for(int j = i; j < i + iir->M; j++){
            y_temp[i+iir->M] -= iir->a[j-i]*y_temp[j];
        }
    }
    //write on output 
    for(int i = 0; i < buffer_size; i++){
        y[i] = y_temp[i+iir->M];
    }
    //update internal buffers
    for(int i = 0; i < iir->M; i++){
        iir->xbuf[i] = x_temp[buffer_size+i];
    }
    for(int i = 0; i < iir->N; i++){
        iir->ybuf[i] = y_temp[buffer_size+i];
    }
}

void free_IIR(IIR* iir){
    //free the memory allocated for the IIR filter
    free(iir->a);
    free(iir->b);
    free(iir->xbuf);
    free(iir->ybuf);
    free(iir);
}