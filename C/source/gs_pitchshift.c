#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gs_pitchshift.h"
#include "utils.h"

GS_pitchshift* init_gs_pitchshift(int shift_factor){
    //allocate the needed memory 
    GS_pitchshift* pitch_shifter = (GS_pitchshift*)malloc(sizeof(GS_pitchshift));
    pitch_shifter->shift_factor = (double*)malloc(sizeof(double));
    pitch_shifter->input_size = (int*)malloc(sizeof(int));
    pitch_shifter->win = (double*)malloc(GRAIN_SIZE*sizeof(double));
    pitch_shifter->len_input_buf = (int*)malloc(sizeof(int));
    pitch_shifter->len_output_buf = (int*)malloc(sizeof(int));

    //test memory allocation
    if(pitch_shifter == NULL || pitch_shifter->shift_factor == NULL || pitch_shifter->input_size == NULL || 
        pitch_shifter->win == NULL || pitch_shifter->len_input_buf == NULL || pitch_shifter->len_output_buf == NULL){
        fprintf(stderr, "Error: Failed to allocate memory for pitch shifter\n");
        free(pitch_shifter->shift_factor);
        free(pitch_shifter->input_size);
        free(pitch_shifter->win);
        free(pitch_shifter->len_input_buf);
        free(pitch_shifter->len_output_buf);
        free(pitch_shifter);
        exit(EXIT_FAILURE);
    }

    //compute the resampling factor
    *pitch_shifter->shift_factor = pitch_factor(shift_factor);
    //the input size needed to get grains of GRAIN_SIZE
    *pitch_shifter->input_size = (int)(GRAIN_SIZE * (*pitch_shifter->shift_factor));
    //the tapering window
    pitch_shifter->win = generate_win(GRAIN_SIZE, FADE);

    //allocate memory for internal buffers 
    pitch_shifter->input_buf = (data_t*)malloc((MAX(*pitch_shifter->input_size, GRAIN_SIZE) + MAX_LEN_BUF)*sizeof(data_t));
    pitch_shifter->output_buf = (data_t*)malloc((MAX(*pitch_shifter->input_size, GRAIN_SIZE) + MAX_LEN_BUF)*sizeof(data_t));
    pitch_shifter->last_grain = (data_t*)malloc(GRAIN_SIZE*sizeof(data_t));

    //test memory allocation 
    if(pitch_shifter->input_buf == NULL || pitch_shifter->output_buf == NULL || pitch_shifter->last_grain == NULL){
        fprintf(stderr, "Error: Failed to allocate memory for pitch shifter\n");
        free(pitch_shifter->input_buf);
        free(pitch_shifter->output_buf);
        free(pitch_shifter->last_grain);
        free(pitch_shifter->shift_factor);
        free(pitch_shifter->input_size);
        free(pitch_shifter->win);
        free(pitch_shifter->len_input_buf);
        free(pitch_shifter->len_output_buf);
        free(pitch_shifter);
        exit(EXIT_FAILURE);
    }
    //initialize the internal buffers
    *pitch_shifter->len_input_buf = 0; 
    memset(pitch_shifter->input_buf, 0.0, (MAX(*pitch_shifter->input_size, GRAIN_SIZE) + MAX_LEN_BUF)*sizeof(data_t));
    //output buffer
    *pitch_shifter->len_output_buf = MAX(*pitch_shifter->input_size, GRAIN_SIZE);
    memset(pitch_shifter->output_buf, 0.0, (MAX(*pitch_shifter->input_size, GRAIN_SIZE) + MAX_LEN_BUF)*sizeof(data_t));
    //last grain stored
    memset(pitch_shifter->last_grain, 0.0, GRAIN_SIZE*sizeof(data_t));

    return pitch_shifter;
}

void reset_gs_pitchshift(GS_pitchshift* pitch_shifter){
    //reset the internal buffers of the pitch shifter
    *pitch_shifter->len_input_buf = 0;
    memset(pitch_shifter->input_buf, 0.0, (MAX(*pitch_shifter->input_size, GRAIN_SIZE) + MAX_LEN_BUF)*sizeof(data_t));
    *pitch_shifter->len_output_buf = GRAIN_SIZE;
    memset(pitch_shifter->output_buf, 0.0, (MAX(*pitch_shifter->input_size, GRAIN_SIZE) + MAX_LEN_BUF)*sizeof(data_t));
    memset(pitch_shifter->last_grain, 0.0, GRAIN_SIZE*sizeof(data_t));
}

void filter_gs_pitchshift(GS_pitchshift* pitch_shifter, data_t* x, data_t* y, int buffer_size){
    //variable to facilitate the reading 
    int input_size = *pitch_shifter->input_size;
    int shift_factor = *pitch_shifter->shift_factor;
    int *len_x = pitch_shifter->len_input_buf;
    int *len_y = pitch_shifter->len_output_buf;
    data_t *xbuf = pitch_shifter->input_buf;
    data_t *ybuf = pitch_shifter->output_buf; 

    //append input to internal input buffer 
    if(*len_x + buffer_size > (MAX(input_size, GRAIN_SIZE) + MAX_LEN_BUF)){
        fprintf(stderr, "Error: internal input buffer of pitchshift is full ! \n");
        exit(EXIT_FAILURE);

    } else {
        
        memcpy(xbuf+(*len_x), x, buffer_size*sizeof(data_t));       
        *len_x += buffer_size;
    }
    
    while (*len_x >= MAX(input_size, GRAIN_SIZE)){ 
        //get a grain
        //temporary variables
        data_t temp_grain[input_size];
        memset(temp_grain, 0.0, input_size*sizeof(data_t));
        data_t grain[GRAIN_SIZE];
        memset(grain, 0.0, GRAIN_SIZE*sizeof(data_t));

        memcpy(temp_grain, xbuf, input_size*sizeof(data_t));
        //resample the grain
        resample(temp_grain, grain, input_size, GRAIN_SIZE, shift_factor);

        //apply the window
        for(int i = 0; i < GRAIN_SIZE; i++){
            grain[i] *= (data_t)(pitch_shifter->win[i]);
        }
        //overlap the grain with the last one
        for(int i = 0; i < OVERLAP; i++){
            grain[i] += pitch_shifter->last_grain[i + JUMP]; 
        }
        //append it to the output buffer 
       
        if (*len_y + JUMP > (MAX(input_size, GRAIN_SIZE) + MAX_LEN_BUF)) {
            fprintf(stderr, "Error: internal output buffer of pitchshift is full ! \n");
            exit(EXIT_FAILURE);

        } else {
            memcpy(ybuf + (*len_y), grain, JUMP*sizeof(data_t));
            *len_y += JUMP;
        }
        //copy current grain into last grain
        memcpy(pitch_shifter->last_grain, grain, GRAIN_SIZE*sizeof(data_t));
        //update input buffer
        memmove(xbuf, xbuf+JUMP, (*len_x-JUMP)*sizeof(data_t));
        *len_x -= JUMP;
    }
    //write result to output
    memcpy(y, ybuf, buffer_size*sizeof(data_t));

    //update output buffer
    memmove(pitch_shifter->output_buf, ybuf+buffer_size, (*len_y-buffer_size)*sizeof(data_t));
    *len_y -= buffer_size;
}

void free_gs_pitchshift(GS_pitchshift* pitch_shifter){
    //free the memory allocated for the pitch shifter
    free(pitch_shifter->win);
    free(pitch_shifter->input_buf);
    free(pitch_shifter->output_buf);
    free(pitch_shifter->last_grain);
    free(pitch_shifter->len_input_buf);
    free(pitch_shifter->len_output_buf);
    free(pitch_shifter->input_size);
    free(pitch_shifter->shift_factor);
    free(pitch_shifter);
}
