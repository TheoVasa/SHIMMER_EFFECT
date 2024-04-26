#include <stdlib.h>

#include "delay_line.h"
#include "utils.h"

DelayLine* init_delay_line(int delay){
    //initialize the delay line
    DelayLine* delay_line = (DelayLine*)malloc(sizeof(DelayLine));
    delay_line->delay = delay;
    delay_line->buffer = (data_t*)malloc(delay*sizeof(data_t));
    return delay_line;
}

void reset_delay_line(DelayLine* delay_line){
    //reset the internal buffers of the delay line
    for(int i = 0; i < delay_line->delay; i++){
        delay_line->buffer[i] = 0;
    }
}

void apply_delay_line(DelayLine* delay_line, data_t* x, data_t* y, int buffer_size){
    //apply the delay line to a signal
    for (int i=0; i<buffer_size; i++){
        if(i < delay_line->delay){
            y[i] = delay_line->buffer[i];
        } else {
            y[i] = x[i-delay_line->delay];
        }
    }
    //update the internal buffer of the delay line
    for(int i = 0; i < delay_line->delay; i++){
        int index = i + buffer_size; 
        if(index < delay_line->delay){
            delay_line->buffer[i] = delay_line->buffer[index];
        } else {
            delay_line->buffer[i] = x[index-delay_line->delay];
        }
    }
}


void free_delay_line(DelayLine* delay_line){
    //free the memory allocated for the delay line
    free(delay_line->buffer);
    free(delay_line);
}

