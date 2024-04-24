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
    shimmer->delay_line = init_delay_line(GRAIN_SIZE);
    shimmer->lowpass = init_butterworth(LOWPASS, parameters->lowcut);
    shimmer->highpass = init_butterworth(HIGHPASS, parameters->highcut);
    shimmer->parameters = parameters;
    shimmer->feedback_buf = (double*)malloc(sizeof(double));
    return shimmer;
}

void reset_shimmer(Shimmer* shimmer){
    //reset all the components 
    reset_gs_pitchshift(shimmer->pitch_shifter);
    reset_schroeder_reverberator(shimmer->reverberator);
    reset_delay_line(shimmer->delay_line);
    reset_butterworth(shimmer->lowpass);
    reset_butterworth(shimmer->highpass);
    shimmer->feedback_buf[0] = 0;
}

void apply_shimmer(Shimmer* shimmer, double* x, double* y, int buffer_size){


}

void free_shimmer(Shimmer* shimmer){
    //free the memory allocated for the shimmer effect
    free_gs_pitchshift(shimmer->pitch_shifter);
    free_schroeder_reverberator(shimmer->reverberator);
    free_delay_line(shimmer->delay_line);
    free_butterworth(shimmer->lowpass);
    free_butterworth(shimmer->highpass);
    free(shimmer->parameters);
    free(shimmer->feedback_buf);
    free(shimmer);
}



