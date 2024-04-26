#include <stdlib.h>

#include "gs_pitchshift.h"
#include "utils.h"

GS_pitchshift* init_gs_pitchshift(int shift_factor){
    //initialize the pitch shifter
    GS_pitchshift* pitch_shifter = (GS_pitchshift*)malloc(sizeof(GS_pitchshift));
    pitch_shifter->shift_factor = shift_factor;
    pitch_shifter->grain_size = GRAIN_SIZE;
    pitch_shifter->fade = FADE;
    pitch_shifter->win = (double*)malloc(pitch_shifter->grain_size*sizeof(double));
    pitch_shifter->input_buf = (double*)malloc(pitch_shifter->grain_size*sizeof(double));
    pitch_shifter->output_buf = (double*)malloc(pitch_shifter->grain_size*sizeof(double));
    pitch_shifter->last_grain = (double*)malloc(pitch_shifter->grain_size*sizeof(double));
    //initialize the window
    hanning(pitch_shifter->win, pitch_shifter->grain_size);
    return pitch_shifter;

}

void reset_gs_pitchshift(GS_pitchshift* pitch_shifter);

void filter_gs_pitchshift(GS_pitchshift* pitch_shifter, double* x, double* y, int buffer_size);


void free_gs_pitchshift(GS_pitchshift* pitch_shifter);
