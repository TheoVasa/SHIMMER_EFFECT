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

    //test memory allocation
    if(shimmer == NULL || shimmer->feedback_buf == NULL){
        fprintf(stderr, "Error: Failed to allocate memory for shimmer\n");
        free(shimmer->feedback_buf);
        free(shimmer);
        exit(EXIT_FAILURE);
    }

    //initialize the components of the shimmer effect
    *shimmer->feedback_buf = 0.0;
    shimmer->pitch_shifter = init_gs_pitchshift(parameters->shift);
    shimmer->reverberator = init_schroeder(parameters->size, parameters->diffusion, parameters->depth, parameters->rate);
    shimmer->delay_line = init_delay_line(GRAIN_SIZE);
    shimmer->lowpass = init_butterworth(LOWPASS, parameters->highcut);
    shimmer->highpass = init_butterworth(HIGHPASS, parameters->lowcut);
    shimmer->parameters = parameters;

    return shimmer;
}

void reset_shimmer(Shimmer* shimmer){
    //reset all the components 
    reset_gs_pitchshift(shimmer->pitch_shifter);
    reset_schroeder(shimmer->reverberator);
    reset_delay_line(shimmer->delay_line);
    reset_butterworth(shimmer->lowpass);
    reset_butterworth(shimmer->highpass);
    *shimmer->feedback_buf = 0.0;
}

void apply_shimmer(Shimmer* shimmer, data_t* x, data_t* y, int buffer_size){
    //init the temporary buffer
    data_t y_temp[buffer_size];
    memset(y_temp, 0.0, (buffer_size) * sizeof(data_t));

    for(int i=0; i<buffer_size; i++){
        data_t feed = (data_t) *shimmer->feedback_buf;
        data_t out = 0.0;
        //apply the pitch shifter
        filter_gs_pitchshift(shimmer->pitch_shifter, &feed, &out, 1);
        //mix feedback with input given the ratio
        out = (data_t)shimmer->parameters->feedback * out + 
                     (data_t) x[i];    
        //apply the reverb 
        data_t rev = 0.0;
        filter_schroeder(shimmer->reverberator, &out, &rev, 1);
        //write output to temporary buffer
        y_temp[i] = rev; 
        //update the feedback buffer
        *shimmer->feedback_buf = rev;

        //OLD DESIGN OF SHIMMER
        /**
        //mix feedback with input given the ratio
        data_t out = (data_t)shimmer->parameters->feedback*(*shimmer->feedback_buf) + 
                     (data_t)(1.0-shimmer->parameters->feedback)*x[i];    
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
        out =   (data_t)(1.0-PITCH_RATIO)*del + 
                (data_t)(PITCH_RATIO*pitch);
        //write output to temporary buffer
        y_temp[i] = out; 
        //update the feedback buffer
        *shimmer->feedback_buf = out;
        **/
    }
    //apply lowpass
    data_t filt[buffer_size];
    filter_butterworth(shimmer->lowpass, y_temp, filt, buffer_size);
    //apply highpass
    filter_butterworth(shimmer->highpass, filt, y_temp, buffer_size);
    //delay the dry input
    data_t del[buffer_size]; 
    memset(del, 0.0, buffer_size*sizeof(data_t)); 
    apply_delay_line(shimmer->delay_line, x, del, buffer_size);
    //mix the dry and wet signals
    for(int i = 0; i < buffer_size; i++){
        y[i] = (data_t)shimmer->parameters->mix*y_temp[i] + 
               (data_t)(1.0-shimmer->parameters->mix)*del[i]; 
    }
}

void free_shimmer(Shimmer* shimmer){
    //free the memory allocated for the shimmer effect
    free_gs_pitchshift(shimmer->pitch_shifter);
    free_schroeder(shimmer->reverberator);
    free_delay_line(shimmer->delay_line);
    free_butterworth(shimmer->lowpass);
    free_butterworth(shimmer->highpass);
    free(shimmer->feedback_buf);
    free(shimmer); 
}
