#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iir.h"
#include "utils.h"
#include "lfo.h"

IIR *init_IIR(Coefficients *a, Coefficients *b, double rate, double depth){
    //initialize the IIR filter
    IIR* iir = (IIR*)malloc(sizeof(IIR));
    iir->xbuf = (data_t*)malloc((*b->order+MAX_BUFFER_SIZE)*sizeof(data_t));
    iir->ybuf = (data_t*)malloc((*a->order+MAX_BUFFER_SIZE)*sizeof(data_t));
    //init the LFO
    iir->lfo = init_lfo(rate, depth); 
    //check if memory allocation was successful
    if(iir == NULL || iir->xbuf == NULL || iir->ybuf == NULL || iir->lfo == NULL){
        fprintf(stderr, "Error: Failed to allocate memory for IIR filter\n");
        free(iir->xbuf);
        free(iir->ybuf);
        free(iir->lfo);
        free(iir);
        exit(EXIT_FAILURE);
    }
    //init the internal buffers
    memset(iir->xbuf, 0.0, (*b->order+MAX_BUFFER_SIZE)*sizeof(data_t));
    memset(iir->ybuf, 0.0, (*a->order+MAX_BUFFER_SIZE)*sizeof(data_t));
    //initialize the coefficients 
    iir->a = a;
    iir->b = b;
    //start the LFO
    start_lfo(iir->lfo);
    return iir;
}

void reset_IIR(IIR* iir){
    //reset the internal buffers of the IIR filter
    memset(iir->xbuf, 0, ((*iir->b->order)+MAX_BUFFER_SIZE) * sizeof(data_t));
    memset(iir->ybuf, 0, ((*iir->a->order)+MAX_BUFFER_SIZE) * sizeof(data_t));
    //reset the lfo
    start_lfo(iir->lfo);
}

void filter_IIR(IIR* iir, data_t* x, data_t* y, int buffer_size){
    //the orders
    int N = *iir->b->order;
    int M = *iir->a->order;
    //number of non-zeros coefficients 
    int N_a = *iir->a->N;
    int N_b = *iir->b->N;

    //append input to internal buffer
    memcpy(iir->xbuf+N, x, buffer_size * sizeof(data_t));
    //append zeros to output buffer
    memset(iir->ybuf+M, 0, buffer_size * sizeof(data_t)); 

    //filter the signal using difference equation of the filter
    for(int i = 0; i < buffer_size; i++){
        iir->ybuf[M + i] = 0; 
        //compute the output
        for(int j = 0; j < N_b; j++){
            double b = iir->b->val[j];
            int d = (int)iir->b->index[j];
            data_t r = iir->xbuf[N + i - d]; 
            //compute the output with the modulation
            iir->ybuf[M + i] += (data_t) (b + lfo_get_value(iir->lfo))* r;
        }
        for(int j = 0; j < N_a; j++){
            double a = iir->a->val[j];
            int d = (int) iir->a->index[j];
            data_t r = iir->ybuf[M + i - d];
            //compute the output with the modulation
            iir->ybuf[M + i] -= (data_t) (a + lfo_get_value(iir->lfo)) * r;
        }  
    }
    //copy the output to the output buffer
    memcpy(y, iir->ybuf + M, buffer_size * sizeof(data_t));

    //update internal buffers
    if(N > 0){
    memmove(iir->xbuf, iir->xbuf + buffer_size, N * sizeof(data_t));
    } 
    memmove(iir->ybuf, iir->ybuf + buffer_size, M * sizeof(data_t));
}


void free_IIR(IIR* iir){
    //free the memory allocated for the IIR filter
    Coefficients *a = iir->a;
    Coefficients *b = iir->b;
    free(a->index);
    free(a->val);
    free(a->N);
    free(a->order);
    free(b->index);
    free(b->val);
    free(b->N);
    free(b->order);
    free(a); 
    free(b); 
    free(iir->xbuf);
    free(iir->ybuf);
    //free the lfo
    free_lfo(iir->lfo);
    free(iir);
}