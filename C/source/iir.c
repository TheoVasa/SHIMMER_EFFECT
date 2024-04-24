#include "iir.h"
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
    for(int i = 0; i < buffer_size; i++){
        double yout = 0;
        iir->xbuf[0] = x[i];
        //compute the output given coefficients and using the difference equation of the IIR filter
        for(int j = 0; j < iir->M; j++){
            yout += iir->a[j]*iir->ybuf[j];
        }
        for(int j = 1; j < iir->N; j++){
            yout += iir->b[j]*iir->xbuf[j];
        }
        for(int j = iir->M-1; j > 0; j--){
            iir->xbuf[j] = iir->ybuf[j-1];
        }
        for(int j = iir->N-1; j > 0; j--){
            iir->ybuf[j] = iir->xbuf[j-1];
        }
        y[i] = yout;
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