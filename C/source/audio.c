#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "shimmer.h"
#include "audio.h"
#include "portaudio.h"
#include "tinywav.h"



int rt_callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData){
    return 0; 
}

int record_callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData){
    return 0; 

}

int playback_callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData){
    return 0; 

}

void audio_process(const char *mode, Shimmer *sh){
    //set the static shimmer pointer 
    shimmer = sh; 

    //start the process given the mode 
    //real-time mode
    if(strcmp(mode, "real-time") == 0){

        //TODO implement real-time mode 

    //record mode
    } else if(strcmp(mode, "record") == 0){

        //TODO implement record mode


    //play-back mode 
    } else if(strcmp(mode, "play-back") == 0){

        printf("====== PLAY-BACK MODE ======\n"); 
        printf("Reading and processing input file %s :  0%%", PATH_TO_INPUT_FILE);

        //read an input file, process it and write input into a file  using the tinywav library [https://github.com/mhroth/tinywav]
        //open the input structure
        TinyWav tw_read;
        tinywav_open_read(&tw_read, 
            PATH_TO_INPUT_FILE,
            TW_SPLIT 
        );
        //read informations
        //the number of channel 
        int num_channel = tw_read.h.NumChannels;
        //the number of samples by channel
        int num_samples_by_channel = tw_read.numFramesInHeader;
        //the sample rate
        int sample_rate = tw_read.h.SampleRate;
        
        //open the write structure
        TinyWav tw_output;
        tinywav_open_write(&tw_output,
            num_channel,
            sample_rate,
            TW_FLOAT32, 
            TW_SPLIT,         
            PATH_TO_OUTPUT_FILE 
        );

        int samples_left = num_samples_by_channel;
        int block_size = BUFFER_SIZE;

        //input and output pointers to correspond to split format
        data_t *samples_input = malloc(block_size * num_channel * sizeof(data_t));
        data_t *samples_output = malloc(block_size *  num_channel * sizeof(data_t));
        data_t **samples_input_ptr = malloc(num_channel * sizeof(data_t *));
        data_t **samples_output_ptr = malloc(num_channel * sizeof(data_t *));
        //test the dynamic allocation
        if(samples_input == NULL || samples_output == NULL || samples_input_ptr == NULL || samples_output_ptr == NULL){
            fprintf(stderr, "Error: Memory allocation failed on samples initialisation\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < num_channel; ++j) {
            samples_input_ptr[j] = samples_input + j*block_size;
            samples_output_ptr[j] = samples_output + j*block_size;
        }

        while(samples_left > 0){
        //read input file
        int samples_read = tinywav_read_f(&tw_read, samples_input_ptr, block_size);
        //process the samples on each channel
        for(int i = 0; i < num_channel; i++){
            apply_shimmer(sh, samples_input_ptr[i], samples_output_ptr[i], samples_read);
        }
        int percent = (int)((1.0 - (float)samples_left / num_samples_by_channel) * 100);
        //print percentage of processing
        printf("\b\b\b%2d%%", percent); 

        //write to output file
        tinywav_write_f(&tw_output, samples_output_ptr, samples_read);
        samples_left -= samples_read; 
        }
        
        //close the reading and writing structure
        tinywav_close_read(&tw_read);
        tinywav_close_write(&tw_output);
        //free the allocated memory 
        free(samples_input);
        free(samples_output);
        free(samples_input_ptr);
        free(samples_output_ptr);

        //print the end of the process
        printf("\rOperation Done !%50s\n", " "); 
        printf("Output file written at %s\n", PATH_TO_OUTPUT_FILE);
        printf("============================\n"); 

    } else {
        fprintf(stderr, "Error: Invalid mode\n");
        exit(EXIT_FAILURE);
    }
}






