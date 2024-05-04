#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "delay_line.h"
#include "utils.h"

DelayLine* init_delay_line(int delay){
    //allocate memory for the delay line
    DelayLine* delay_line = (DelayLine*)malloc(sizeof(DelayLine));
    delay_line->delay = (int*)malloc(sizeof(int));
    delay_line->buffer = (data_t*)malloc(delay*sizeof(data_t));
    //check if memory allocation was successful
    if(delay_line->buffer == NULL || delay_line == NULL || delay_line->delay == NULL){
        fprintf(stderr, "Error: Failed to allocate memory for delay line\n");
        free(delay_line->delay);
        free(delay_line->buffer);
        free(delay_line);
        exit(EXIT_FAILURE);
    }
    *delay_line->delay = delay;
    memset(delay_line->buffer, 0.0, delay * sizeof(data_t));
    return delay_line;
}

void reset_delay_line(DelayLine* delay_line){
    //reset the internal buffers of the delay line
    memset(delay_line->buffer, 0, *delay_line->delay * sizeof(data_t));
}

void apply_delay_line(DelayLine* delay_line, data_t* x, data_t* y, int buffer_size){
    //append the internal buffer and the input to a temporary buffer
    int delay = *delay_line->delay;
    data_t buf_temp[buffer_size + delay];
    memset(buf_temp, 0.0, (buffer_size + delay) * sizeof(data_t));
    memcpy(buf_temp, delay_line->buffer, delay * sizeof(data_t));
    memcpy(buf_temp + delay, x, buffer_size * sizeof(data_t));
    //write in output
    memcpy(y, buf_temp, buffer_size * sizeof(data_t));
    //update internal buffer 
    memcpy(delay_line->buffer, buf_temp + buffer_size, delay * sizeof(data_t));
}

void free_delay_line(DelayLine* delay_line){
    //free the memory allocated for the delay line
    free(delay_line->buffer);
    free(delay_line->delay);
    free(delay_line);
}
