#include <stdlib.h>
#include <string.h> 
#include <stdio.h>

#include "shimmer.h"
#include "schroeder_reverberator.h"
#include "gs_pitchshift.h"
#include "butterworth.h"
#include "delay_line.h"
#include "utils.h"

Shimmer* init_shimmer(Parameters *parameters){
    Shimmer* shimmer = (Shimmer*)malloc(sizeof(Shimmer));
    shimmer->feedback_buf = 0;

    //initialize the components of the shimmer effect
    shimmer->pitch_shifter = init_gs_pitchshift(parameters->shift);
    shimmer->reverberator = init_schroeder(parameters->size, parameters->diffusion);
    shimmer->delay_line1 = init_delay_line(GRAIN_SIZE);
    shimmer->delay_line2 = init_delay_line(GRAIN_SIZE);
    shimmer->lowpass = init_butterworth(LOWPASS, parameters->lowcut);
    shimmer->highpass = init_butterworth(HIGHPASS, parameters->highcut);
    shimmer->parameters = parameters;

    return shimmer;
}

void reset_shimmer(Shimmer* shimmer){
    //reset all the components 
    reset_gs_pitchshift(shimmer->pitch_shifter);
    reset_schroeder(shimmer->reverberator);
    reset_delay_line(shimmer->delay_line1);
    reset_delay_line(shimmer->delay_line2);
    reset_butterworth(shimmer->lowpass);
    reset_butterworth(shimmer->highpass);
    shimmer->feedback_buf = 0.0;
}

void apply_shimmer(Shimmer* shimmer, data_t* x, data_t* y, int buffer_size){
     
    data_t y_temp[buffer_size];
    memset(y_temp, 0.0, (buffer_size) * sizeof(data_t));

    for(int i=0; i<buffer_size; i++){
        //mix feedback with input given the ratio
        data_t f = (data_t)shimmer->parameters->feedback* (data_t)shimmer->feedback_buf; 
        data_t d = (data_t)(1.0-shimmer->parameters->feedback)*x[i];
        data_t out = f + d; 
        //apply the reverb 
        data_t rev = 0.0;
        filter_schroeder(shimmer->reverberator, &out, &rev, 1); 
        //apply the pitch shifter
        data_t pitch = 0.0; 
        filter_gs_pitchshift(shimmer->pitch_shifter, &rev, &pitch, 1);
        //mix the pitched signal and and the input given the ratio
        //we need to delay the input to match the output of the pitchshift
        data_t del = 0.0; 
        apply_delay_line(shimmer->delay_line1, &rev, &del, 1);
        out = (data_t)(1.0-PITCH_RATIO)*del + (data_t)(PITCH_RATIO*pitch);
        //write output to temporary buffer
        y_temp[i] = out; 
        shimmer->feedback_buf = out;
    }
    printf("feedback: %f\n", shimmer->feedback_buf);
    //apply lowpass
    //filter_butterworth(shimmer->lowpass, y, y, buffer_size);
    //apply highpass
    //filter_butterworth(shimmer->highpass, y, y, buffer_size);
    //delay the dry input
    data_t del[buffer_size]; 
    memset(del, 0.0, buffer_size*sizeof(data_t)); 
    apply_delay_line(shimmer->delay_line2, x, del, buffer_size);
    //mix the dry and wet signals
    
    for(int i = 0; i < buffer_size; i++){
        data_t w = (data_t)shimmer->parameters->mix*y_temp[i]; 
        data_t d = (data_t)(1.0-shimmer->parameters->mix)*x[i];
        y[i] = w + d; 
    }
    
}

void free_shimmer(Shimmer* shimmer){
    //free the memory allocated for the shimmer effect
    free_gs_pitchshift(shimmer->pitch_shifter);
    free_schroeder(shimmer->reverberator);
    free_delay_line(shimmer->delay_line1);
    free_delay_line(shimmer->delay_line2);
    free_butterworth(shimmer->lowpass);
    free_butterworth(shimmer->highpass);
    free(shimmer); 
}
