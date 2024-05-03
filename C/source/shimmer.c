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
    shimmer->feedback_buf = (data_t*)malloc(sizeof(data_t));
    //check if memory allocation was successful
    if(shimmer == NULL || shimmer->feedback_buf == NULL){
        fprintf(stderr, "Error: Failed to allocate memory for shimmer\n");
        free(shimmer->feedback_buf);
        free(shimmer);
        exit(EXIT_FAILURE);
    }

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
    shimmer->feedback_buf[0] = (data_t)0;
}

void apply_shimmer(Shimmer* shimmer, data_t* x, data_t* y, int buffer_size){
    filter_gs_pitchshift(shimmer->pitch_shifter, x, y, buffer_size);
    /**
     
    data_t y_temp[buffer_size + 1];
    memset(y_temp, 0, (buffer_size + 1) * sizeof(data_t));
    //append data into the temp buffer
    memcpy(y_temp, shimmer->feedback_buf, sizeof(data_t));
    memcpy(y_temp + 1, x, buffer_size * sizeof(data_t));
    

    for(int i=0; i<buffer_size; i++){
        //mix feedback with input given the ratio
        data_t out = (data_t)(shimmer->parameters->feedback*x[i] + (1-shimmer->parameters->feedback)*y[i]);
        //apply the reverb 
        filter_schroeder(shimmer->reverberator, &out, &out, 1); 
        //apply the pitch shifter
        data_t pitch; 
        filter_gs_pitchshift(shimmer->pitch_shifter, &out, &pitch, 1);
        //mix the pitched signal and and the input given the ratio
        //we need to delay the input to match the output of the pitchshift
        apply_delay_line(shimmer->delay_line1, &out, &out, 1);
        out = (data_t)((1-PITCH_RATIO)*out + PITCH_RATIO*pitch);
        //write output to temporary buffer
        y_temp[i+1] = out; 
    }
    //update internal buffer
    shimmer->feedback_buf[0] = y_temp[buffer_size];
    //update output buffer
    memcpy(y, y_temp + 1, buffer_size * sizeof(data_t));

    //apply lowpass
    //filter_butterworth(shimmer->lowpass, y, y, buffer_size);
    //apply highpass
    //filter_butterworth(shimmer->highpass, y, y, buffer_size);
    //delay the dry input
    apply_delay_line(shimmer->delay_line2, x, x, buffer_size);
    //mix the dry and wet signals
    for(int i=0; i<buffer_size; i++){
        y[i] = (data_t)(1-shimmer->parameters->mix)*x[i] + shimmer->parameters->mix*y[i];
    }

    **/
}

void free_shimmer(Shimmer* shimmer){
    //free the memory allocated for the shimmer effect
    free_gs_pitchshift(shimmer->pitch_shifter);
    free_schroeder(shimmer->reverberator);
    free_delay_line(shimmer->delay_line1);
    free_delay_line(shimmer->delay_line2);
    free_butterworth(shimmer->lowpass);
    free_butterworth(shimmer->highpass);
    free(shimmer->parameters);
    free(shimmer->feedback_buf);
    free(shimmer);
}
