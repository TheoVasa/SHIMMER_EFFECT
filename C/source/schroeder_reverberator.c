
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schroeder_reverberator.h"
#include "utils.h"
#include "iir.h"

Schroeder_reverberator *init_schroeder(double size, double diffusion){
    //allocate memory
    Schroeder_reverberator* schroeder = (Schroeder_reverberator*)malloc(sizeof(Schroeder_reverberator));
    if (schroeder == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for Schroeder_reverberator\n");
        exit(EXIT_FAILURE);
    }

    //compute the parameters of the reverb 
    //------------------------------------
    //find number of all passes filters given diffusion parameter
    int N_ap = MIN_AP + (int)(((double)(MAX_AP-MIN_AP)*diffusion)/10.0);

    //the parameters of the all-pass and comb filters
    double g_ap[N_ap]; 
    double d_ap[N_ap];
    double g_c[N_C];
    double d_c[N_C];
    
    //compute the all-pass delays parameters (we take N_ap primes number between 2 and 113 as equi-distant from each other as possible)
    int shift = (int)PRIME_COUNT / N_ap;
    int primes[PRIME_COUNT] = PRIMES; 
    for (int i = 0; i < N_ap; i++) { 
        g_ap[i] = BASIC_GAIN_AP;
        d_ap[i] = primes[i*shift];
    }
        
    //compute the comb filter parameters
    //the basic gains and delays are shifted by a factor proportional to the size parameter
    double shift_gain = size * MAX_SHIFT_GAIN_C;
    double shift_delay = size * MAX_SHIFT_DELAY_C;
    double *basic_gains_c = (double[])BASIC_GAINS_C;
    double *basic_delays_c = (double[])BASIC_DELAYS_C;
    for (int i = 0; i < N_C; i++) {
        g_c[i] = basic_gains_c[i] + shift_gain;
        d_c[i] = basic_delays_c[i] + shift_delay;
    }

    //create the all pass filters
    IIR **allpasses = (IIR**)malloc(N_ap*sizeof(IIR*)); 
    for(int i = 0; i < N_ap; i++){
        allpasses[i] = generate_allpass(g_ap[i], d_ap[i]);
    }
    //create the comb filters
    IIR **combs = (IIR**)malloc(N_C*sizeof(IIR*));
    for(int i = 0; i < N_C; i++){
        combs[i] = generate_comb(g_c[i], d_c[i]);
    }

    //init the components of the reverberator
    schroeder->allpasses = allpasses;
    schroeder->combs = combs;
    schroeder->n_ap = N_ap;
    schroeder->n_c = N_C;
    schroeder->wet = WET_REVERB;
    return schroeder;
}

void reset_schroeder(Schroeder_reverberator* schroeder){
    //reset all the all-pass and comb filters
    for(int i = 0; i < schroeder->n_ap; i++){
      reset_IIR(schroeder->allpasses[i]);
    }
    for(int i = 0; i < schroeder->n_c; i++){
        reset_IIR(schroeder->combs[i]);
    }
}

void filter_schroeder(Schroeder_reverberator* schroeder, data_t* x, data_t* y, int buffer_size){
    //temporary variables
    data_t reverb_wet[buffer_size];
    memset(reverb_wet, 0.0, buffer_size*sizeof(data_t));

    //apply the comb_filters in parallel
    for(int i = 0; i < schroeder->n_c; i++){ 
        data_t temp[buffer_size];
        memset(temp, 0.0, buffer_size*sizeof(data_t));
        filter_IIR(schroeder->combs[i], x, temp, buffer_size);
        for(int j = 0; j < buffer_size; j++){
            reverb_wet[j] += (1/(double)schroeder->n_c)*temp[j];
        }
    }
   
    //apply the all-pass filters in series
    for(int i = 0; i < schroeder->n_ap; i++){
        data_t temp[buffer_size];
        memset(temp, 0.0, buffer_size*sizeof(data_t));
        filter_IIR(schroeder->allpasses[i], reverb_wet, temp, buffer_size);
        memcpy(reverb_wet, temp, buffer_size*sizeof(data_t));
    }
    memcpy(y, reverb_wet, buffer_size*sizeof(data_t)); 
    //mix the wet and dry signals 
    for(int i = 0; i < buffer_size; i++){
        data_t wet = (data_t)schroeder->wet*reverb_wet[i]; 
        data_t dry = (data_t)(1.0-schroeder->wet)*x[i];
        y[i] = wet + dry; 
    }
}

void free_schroeder(Schroeder_reverberator* schroeder){
    //free all the all-pass and comb filters
    for(int i = 0; i < schroeder->n_ap; i++){
        free_IIR(schroeder->allpasses[i]);
    }
    for(int i = 0; i < schroeder->n_c; i++){
        free_IIR(schroeder->combs[i]);
    }
    //free the struct
    free(schroeder->allpasses);
    free(schroeder->combs);
    free(schroeder); 
}

IIR* generate_allpass(double gain, double delay){
    Coefficients *a = (Coefficients*)malloc(sizeof(Coefficients));
    Coefficients *b = (Coefficients*)malloc(sizeof(Coefficients));
    //compute the length in samples given the delay in ms
    int m = time_to_samples(delay); 
    //init the memory needed to store the coefficients
    a->order = m+1;   
    b->order = m+1;
    a->index = (int*)malloc(sizeof(int));
    a->val = (double*)malloc(sizeof(double));
    a->N = 1;
    b->index = (int*)malloc(2*sizeof(int));
    b->val = (double*)malloc(2*sizeof(double));
    b->N = 2;
    //check if memory allocation was successful
    if(a == NULL || b == NULL || a->index == NULL || a->val == NULL || b->index == NULL || b->val == NULL){
        fprintf(stderr, "Error: Failed to allocate memory for allpass filter\n");
        free(a->index);
        free(a->val);
        free(b->index);
        free(b->val);
        free(a);
        free(b);
        exit(EXIT_FAILURE);
    }
    //set the coefficients
    a->index[0] = m;
    a->val[0] = -gain;
    b->index[0] = 0;
    b->val[0] = -gain;
    b->index[1] = m;
    b->val[1] = 1;

    return init_IIR(a, b);
}

IIR* generate_comb(double gain, double delay){
    Coefficients *a = (Coefficients*)malloc(sizeof(Coefficients));
    Coefficients *b = (Coefficients*)malloc(sizeof(Coefficients));
    //compute the length in samples given the delay in ms
    int m = time_to_samples(delay); 
    //init the memory needed to store the coefficients
    a->order = m;   
    b->order = 1;
    a->index = (int*)malloc(sizeof(int));
    a->val = (double*)malloc(sizeof(double));
    a->N = 1;
    b->index = (int*)malloc(sizeof(int));
    b->val = (double*)malloc(sizeof(double));
    b->N = 1;
    //check if memory allocation was successful
    if(a == NULL || b == NULL || a->index == NULL || a->val == NULL || b->index == NULL || b->val == NULL){
        fprintf(stderr, "Error: Failed to allocate memory for comb filter\n");
        free(a->index);
        free(a->val);
        free(b->index);
        free(b->val);
        free(a);
        free(b);
        exit(EXIT_FAILURE);
    }
    //set the coefficients
    a->index[0] = m;
    a->val[0] = -gain;
    b->index[0] = 0;
    b->val[0] = 1;

    return init_IIR(a, b);
}