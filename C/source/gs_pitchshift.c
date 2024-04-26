#include <stdlib.h>
#include <string.h>

#include "gs_pitchshift.h"
#include "utils.h"

GS_pitchshift* init_gs_pitchshift(int shift_factor){
    //initialize the pitch shifter
    GS_pitchshift* pitch_shifter = (GS_pitchshift*)malloc(sizeof(GS_pitchshift));

    //compute the resampling factor
    pitch_shifter->shift_factor = pitch_factor(shift_factor);
    //the input size needed to get grains of GRAIN_SIZE
    pitch_shifter->input_size = (int)(GRAIN_SIZE * pitch_shifter->shift_factor);

    //the tapering window
    pitch_shifter->win = generate_win(GRAIN_SIZE, FADE);

    //internal buffers
    //input buffer
    pitch_shifter->len_input_buf = 0; 
    pitch_shifter->input_buf = (data_t*)malloc(MAX_LEN_BUF*sizeof(data_t));
    //output buffer
    pitch_shifter->len_output_buf = GRAIN_SIZE;
    pitch_shifter->output_buf = (data_t*)malloc(MAX_LEN_BUF*sizeof(data_t));
    //last grain stored
    pitch_shifter->last_grain = (data_t*)malloc(GRAIN_SIZE*sizeof(data_t));
    return pitch_shifter;

}

void reset_gs_pitchshift(GS_pitchshift* pitch_shifter){
    //reset the internal buffers of the pitch shifter
    pitch_shifter->len_input_buf = 0;
    memset(pitch_shifter->input_buf, 0, MAX_LEN_BUF*sizeof(data_t));
    pitch_shifter->len_output_buf = GRAIN_SIZE;
    memset(pitch_shifter->output_buf, 0, MAX_LEN_BUF*sizeof(data_t));
    memset(pitch_shifter->last_grain, 0, GRAIN_SIZE*sizeof(data_t));
}

void filter_gs_pitchshift(GS_pitchshift* pitch_shifter, data_t* x, data_t* y, int buffer_size){
    //append input to internal input buffer 
    if(pitch_shifter->len_input_buf + buffer_size > MAX_LEN_BUF){
        //the buffer is full
        printf("Input buffer is full\n");
    } else {
        memcpy(pitch_shifter->input_buf+pitch_shifter->len_input_buf, x, buffer_size*sizeof(data_t));
        pitch_shifter->len_input_buf += buffer_size;
    }

    while (pitch_shifter->len_input_buf >= MAX(pitch_shifter->input_size, GRAIN_SIZE)){
        //get a grain
        data_t temp_grain[pitch_shifter->input_size];
        data_t grain[GRAIN_SIZE];
        memcpy(temp_grain, pitch_shifter->input_buf, pitch_shifter->input_size*sizeof(data_t));
        //resample the grain
        resample(temp_grain, grain, pitch_shifter->input_size, GRAIN_SIZE, pitch_shifter->shift_factor);

        //apply the window
        for(int i = 0; i < GRAIN_SIZE; i++){
            grain[i] *= (data_t)(pitch_shifter->win[i]);
        }
        //overlap the grain with the last one
        for(int i = 0; i < OVERLAP; i++){
            grain[i] += pitch_shifter->last_grain[i + JUMP]; 
        }
        //append it to the output buffer 
       
        if (pitch_shifter->len_output_buf + JUMP > MAX_LEN_BUF) {
            // the buffer is full
            printf("Output buffer is full\n");
        } else {
            memcpy(pitch_shifter->output_buf + pitch_shifter->len_output_buf, grain, JUMP*sizeof(data_t));
            pitch_shifter->len_output_buf += JUMP;
        }

        //copy current grain into last grain
        memcpy(pitch_shifter->last_grain, grain, GRAIN_SIZE*sizeof(data_t));

        //update input buffer
        memcpy(pitch_shifter->input_buf, pitch_shifter->input_buf+JUMP, (pitch_shifter->len_input_buf-JUMP)*sizeof(data_t));
        pitch_shifter->len_input_buf -= JUMP;
    }
    //write result to output
    memcpy(y, pitch_shifter->output_buf, buffer_size*sizeof(data_t));
    //update output buffer
    memcpy(pitch_shifter->output_buf, pitch_shifter->output_buf+buffer_size, (pitch_shifter->len_output_buf-buffer_size)*sizeof(data_t));
    pitch_shifter->len_output_buf -= buffer_size;
}

void free_gs_pitchshift(GS_pitchshift* pitch_shifter){
    //free the memory allocated for the pitch shifter
    free(pitch_shifter->win);
    free(pitch_shifter->input_buf);
    free(pitch_shifter->output_buf);
    free(pitch_shifter->last_grain);
    free(pitch_shifter);
}
