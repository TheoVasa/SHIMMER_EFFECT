#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "shimmer.h"
#include "audio.h"
#include "portaudio.h"
#include "tinywav.h"
#include "delay_line.h"



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
        printf("====== REAL-TIME MODE ======\n");
        printf("============================\n"); 
    //play-back mode 
    } else if(strcmp(mode, "play-back") == 0){

        printf("====== PLAY-BACK MODE ======\n"); 
        //ask the user if he wants stero or mono output 
        int num_channel_out = 0; 
        printf("Do you want stereo output ? [y/n]\n");
        char answer[10];
        scanf("%s", answer);
        while(strcmp(answer, "y") != 0 && strcmp(answer, "n") != 0){
            printf("Invalid answer. Please enter y or n :");
            scanf("%s", answer);
        }
        if(strcmp(answer, "y") == 0){
            num_channel_out = 2;
        } else if (strcmp(answer, "n") == 0){
            num_channel_out = 1;
        }
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
        //check if we have mono output
        if(num_channel != 1){
            fprintf(stderr, "Error: The input file must be mono !\n");
            exit(EXIT_FAILURE);
        }
        //the number of samples by channel
        int num_samples_by_channel = tw_read.numFramesInHeader;
        //the sample rate
        int sample_rate = tw_read.h.SampleRate;
              
        //open the write structure
        TinyWav tw_output;
        tinywav_open_write(&tw_output,
            num_channel_out,
            sample_rate,
            TW_FLOAT32, 
            TW_SPLIT,         
            PATH_TO_OUTPUT_FILE 
        );

        int samples_left = num_samples_by_channel;
        int block_size = BUFFER_SIZE;
        //if we want stereo output, init a delay line to emulate stereo output 
        DelayLine *delay_line = init_delay_line(time_to_samples(STEREO_OFFSET));
        
        //input and output pointers to correspond to split format
        data_t *samples_input = malloc(block_size * sizeof(data_t));
        data_t *samples_output = malloc(block_size * num_channel_out * sizeof(data_t));
        data_t **samples_input_ptr = malloc(sizeof(data_t *));
        data_t **samples_output_ptr = malloc(num_channel_out * sizeof(data_t *));
        //test the dynamic allocation
        if(samples_input == NULL || samples_output == NULL || samples_input_ptr == NULL || samples_output_ptr == NULL){
            fprintf(stderr, "Error: Memory allocation failed on samples initialisation\n");
            exit(EXIT_FAILURE);
        }
        //set the pointers
        samples_input_ptr[0] = samples_input;
        for (int j = 0; j < num_channel_out; ++j) {
            samples_output_ptr[j] = samples_output + j*block_size;
        }

        while(samples_left > 0){
        //read input file
        int samples_read = tinywav_read_f(&tw_read, samples_input_ptr, block_size);
        //process the samples on the first channel (mono signals)
        apply_shimmer(sh, samples_input_ptr[0], samples_output_ptr[0], samples_read);
        //write to second channel if we have stereo output
        if(num_channel_out == 2){
            apply_delay_line(delay_line, samples_output_ptr[0], samples_output_ptr[1], samples_read);
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






