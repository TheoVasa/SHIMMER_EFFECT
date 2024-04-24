
#include <stdio.h>
#include <stdlib.h>

#include "schroeder_reverberator.h"
#include "utils.h"
#include "iir.h"

Schroeder_reverberator *init_schroeder(double size, double diffusion){

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

    printf("All-pass gains: ");
    for (int i = 0; i < 9; i++)
        printf("%f ", g_ap[i]);
    printf("\nAll-pass delays: ");
    for (int i = 0; i < 9; i++)
        printf("%f ", d_ap[i]);
    printf("\nComb filter gains: ");
    for (int i = 0; i < 4; i++)
        printf("%f ", g_c[i]);
    printf("\nComb filter delays: ");
    for (int i = 0; i < 4; i++)
        printf("%f ", d_c[i]);

    //------------------------------------
    //create the all pass filters
    IIR* allpasses[N_ap];
    for(int i = 0; i < N_ap; i++){
        allpasses[i] = generate_allpass(g_ap[i], d_ap[i]);
    }
    //create the comb filters
    IIR* combs[N_C];
    for(int i = 0; i < N_C; i++){
        combs[i] = generate_comb(g_c[i], d_c[i]);
    }
    //create the struct
    Schroeder_reverberator* schroeder = (Schroeder_reverberator*)malloc(sizeof(Schroeder_reverberator));
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

void filter_schroeder(Schroeder_reverberator* schroeder, double* x, double* y, int buffer_size){
    //temporary variables
    double reverb_wet[buffer_size];
    double temp[buffer_size];

    //apply the comb_filters in parallel
    for(int i = 0; i < schroeder->n_c; i++){
        filter_IIR(schroeder->combs[i], x, temp, buffer_size);
        for(int j = 0; j < buffer_size; j++){
            reverb_wet[j] += temp[j];
        }
    }
    //apply the all-pass filters in series
    for(int i = 0; i < schroeder->n_ap; i++){
        filter_IIR(schroeder->allpasses[i], reverb_wet, temp, buffer_size);
        for(int j = 0; j < buffer_size; j++){
            reverb_wet[j] = temp[j];
        }
    }
    //mix the wet and dry signals
    for(int i = 0; i < buffer_size; i++){
        y[i] = schroeder->wet*x[i] + (1-schroeder->wet)*y[i];
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
    free(schroeder); 
}

IIR* generate_allpass(double gain, double delay){
    //compute the length in samples given the delay in ms
    int m = time_to_samples(delay); 
    //the coefficients of the transfer function
    double a[m+1];  
    double b[m+1];
    a[m] = -gain; 
    b[0] = -gain; 
    b[m] = 1; 
    return init_IIR(a, m+1, b, m+1);
}

IIR* generate_comb(double gain, double delay){
    //compute the length in samples given the delay in ms
    int m = time_to_samples(delay); 
    int n = 1; 
    //the coefficients of the transfer function
    double a[m+1];  
    double b[n];
    a[m] = -gain; 
    b[0] = 1; 
    return init_IIR(a, m+1, b, n);
}