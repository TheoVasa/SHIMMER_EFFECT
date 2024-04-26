#include <stdlib.h>

#include "shimmer.h"
#include "schroeder_reverberator.h"
#include "gs_pitchshift.h"
#include "butterworth.h"
#include "delay_line.h"
#include "utils.h"


Shimmer* init_shimmer(Parameters *parameters){
    Shimmer* shimmer = (Shimmer*)malloc(sizeof(Shimmer));

    //initialize the components of the shimmer effect
    shimmer->pitch_shifter = init_gs_pitchshift(parameters->shift);
    shimmer->reverberator = init_schroeder(parameters->size, parameters->diffusion);
    shimmer->delay_line1 = init_delay_line(GRAIN_SIZE);
    shimmer->delay_line2 = init_delay_line(GRAIN_SIZE);
    shimmer->lowpass = init_butterworth(LOWPASS, parameters->lowcut);
    shimmer->highpass = init_butterworth(HIGHPASS, parameters->highcut);
    shimmer->parameters = parameters;
    shimmer->feedback_buf = (double*)malloc(sizeof(double));
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
    shimmer->feedback_buf[0] = 0;
}

void apply_shimmer(Shimmer* shimmer, double* x, double* y, int buffer_size){
    double y_temp[buffer_size + 1];
    //append data into the temp buffer
    for(int i=0; i<buffer_size + 1; i++){
        if(i == 0){
            y_temp[i] = shimmer->feedback_buf[i];
        } else {
            y_temp[i] = x[i];   
        }
    }
    for(int i=0; i<buffer_size; i++){
        double out; 
        //mix feedback with input given the ratio
        out = shimmer->parameters->feedback*x[i] + (1-shimmer->parameters->feedback)*y[i];
        //apply the reverb
        filter_schroeder(shimmer->reverberator, &out, &out, 1); 
        //apply the pitch shifter
        double pitch; 
        filter_gs_pitchshift(shimmer->pitch_shifter, &out, &pitch, 1);
        //mix the pitched signal and and the input given the ratio
        //we need to delay the input to match the output of the pitchshift
        apply_delay_line(shimmer->delay_line1, &out, &out, 1);
        out = (1-PITCH_RATIO)*out + PITCH_RATIO*pitch;
        //write output to temporary buffer
        y_temp[i+1] = out; 
    }
    //update internal buffer
    shimmer->feedback_buf[0] = y_temp[buffer_size];
    //update output buffer
    for(int i=0; i<buffer_size; i++){
        y[i] = y_temp[i+1];
    }
    //apply lowpass
    filter_butterworth(shimmer->lowpass, y, y, buffer_size);
    //apply highpass
    filter_butterworth(shimmer->highpass, y, y, buffer_size);
    //delay the dry input
    apply_delay_line(shimmer->delay_line2, x, x, buffer_size);
    //mix the dry and wet signals
    for(int i=0; i<buffer_size; i++){
        y[i] = (1-shimmer->parameters->mix)*x[i] + shimmer->parameters->mix*y[i];
    }
}

void free_shimmer(Shimmer* shimmer){
    //free the memory allocated for the shimmer effect
    free_gs_pitchshift(shimmer->pitch_shifter);
    free_schroeder(shimmer->reverberator);
    free_delay_line(shimmer->delay_line1);
    free_delay_line(shimmer->delay_line1);
    free_butterworth(shimmer->lowpass);
    free_butterworth(shimmer->highpass);
    free(shimmer->parameters);
    free(shimmer->feedback_buf);
    free(shimmer);
}



