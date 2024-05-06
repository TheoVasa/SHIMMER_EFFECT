#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils.h"
#include "shimmer.h"
#include "audio.h"
#include "portaudio.h"
#include "tinywav.h"
#include "delay_line.h"


int rt_callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData){
    //cast the user data
    UserData *data = (UserData*)userData;
    //cast the input and output buffer
    data_t *in = (data_t*)inputBuffer;
    data_t *out = (data_t*)outputBuffer;
    if(data->numOutputChannels == 2){
        data_t left[framesPerBuffer];
        data_t right[framesPerBuffer];
        apply_shimmer(data->shimmer, in, left, framesPerBuffer);
        //delay right channel to emulate stereo output
        apply_delay_line(data->delayLine, left, right, framesPerBuffer);
        //interleave the left and right channels
        for (int i = 0; i < framesPerBuffer; ++i) {
            out[2*i] = left[i];
            out[2*i + 1] = right[i];
        }
    } else {
        //process the input buffer using the shimmer effect
        apply_shimmer(data->shimmer, in, out, framesPerBuffer);
    }
    //return the result of the callback
    return paContinue;
}

void audio_process(const char *mode, Shimmer *sh){
    //start the process given the mode 
    //real-time mode
    if(strcmp(mode, "real-time") == 0){
        printf("====== REAL-TIME MODE ======\n");
        // Initialize PortAudio
        PaError err = Pa_Initialize();
        if (err != paNoError) {
            fprintf(stderr, "Error: Failed to initialize PortAudio\n");
            exit(EXIT_FAILURE);
        }

    //set the user data structure 
    UserData data;
    data.shimmer = sh;
    data.delayLine = init_delay_line(time_to_samples(STEREO_OFFSET));
    //ask the user if he wants stereo or mono output
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
    data.numOutputChannels = num_channel_out;
    // Set up the input and output parameters
    PaStreamParameters inputParameters, outputParameters;
    inputParameters.device = Pa_GetDefaultInputDevice();
    inputParameters.channelCount = 1; // Mono input
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.channelCount = num_channel_out; 
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    // Open the audio stream
    PaStream *stream;
    err = Pa_OpenStream(&stream, &inputParameters, &outputParameters, SAMPLE_RATE, 0, paNoFlag, &rt_callback, &data);
    if (err != paNoError) {
        fprintf(stderr, "Error: Failed to open audio stream\n");
        exit(EXIT_FAILURE);
    }

    // Start the audio stream
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        fprintf(stderr, "Error: Failed to start audio stream\n");
        exit(EXIT_FAILURE);
    }
    //inform the user that the process is starting
    printf("----------------------------\n");
    printf("Starting real-time audio processing\n\n");
    printf("The input and output devices are the one marked as default on your computer audio settings\n\n"); 
    printf("/!/ If you are using a microphone, wear headphones to avoid feedback and larsen effect/!/\n\n");
    // Wait for the user to type stop
    printf("Press 'stop' then <enter> to quit\n");
    printf("----------------------------\n");
    char stop[10];
    scanf("%s", stop);
    while(strcmp(stop, "stop") != 0){
        printf("Invalid command. Please enter 'stop' to stop the process\n");
        scanf("%s", stop);
    }

    // Stop and close the audio stream
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        fprintf(stderr, "Error: Failed to stop audio stream\n");
        exit(EXIT_FAILURE);
    }

    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        fprintf(stderr, "Error: Failed to close audio stream\n");
        exit(EXIT_FAILURE);
    }

    // Terminate PortAudio
    Pa_Terminate();

    //free the allocated memory
    free_delay_line(data.delayLine);
    free_shimmer(sh);
    printf("============================\n"); 
    //-----------------------------------------------------------------
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
        //ask the user the name of the input file
        char input_file_name[24];
        printf("Enter the name of the input file (max 20 char): ");
        scanf("%s", input_file_name);
        strcat(input_file_name, ".wav");
        //create the path to the input and output file
        char path_to_input_file[50] = PATH_TO_AUDIO; 
        strcat(path_to_input_file, "/");
        strcat(path_to_input_file, input_file_name);

        //check if the file exists
        FILE *file = fopen(path_to_input_file, "r");
        while (file == NULL) {
            printf("File not found. Please enter a valid input file name: ");
            scanf("%s", input_file_name);
            strcat(input_file_name, ".wav");
            strcpy(path_to_input_file, PATH_TO_AUDIO);
            strcat(path_to_input_file, "/");
            strcat(path_to_input_file, input_file_name);
            file = fopen(path_to_input_file, "r");
        }
        fclose(file);

        //ask the user the name of the output file
        char output_file_name[24];
        printf("Enter the name of the output file (max 20 char): ");
        scanf("%s", output_file_name);
        strcat(output_file_name, ".wav");
        
        char path_to_output_file[50] = PATH_TO_AUDIO; 
        strcat(path_to_output_file, "/");
        strcat(path_to_output_file, output_file_name);

        //print all the informations given by the user 
        printf("----------------------------\n");
        printf("Input file : %s\n", path_to_input_file);
        printf("Output file : %s\n", path_to_output_file);
        if(num_channel_out == 1){
            printf("Output mode : Mono\n");
        } else if(num_channel_out == 2){
            printf("Output mode : Stereo\n");
        }
        
        //read an input file, process it and write input into a file  using the tinywav library [https://github.com/mhroth/tinywav]
        //open the input structure
        TinyWav tw_read;
        tinywav_open_read(&tw_read, 
            path_to_input_file,
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
        //print the informations of the input file
        printf("Number of samples : %d\n", num_samples_by_channel);
        printf("Sample rate : %d\n", sample_rate);
              
        //open the write structure
        TinyWav tw_output;
        tinywav_open_write(&tw_output,
            num_channel_out,
            sample_rate,
            TW_FLOAT32, 
            TW_SPLIT,         
            path_to_output_file 
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
        printf("----------------------------\n");
        printf("Reading and processing input file :  0%%");

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
        free_delay_line(delay_line);
        free_shimmer(sh);
        //print the end of the process
        printf("\rOperation Done !%50s\n", " "); 
        printf("Output file written at the given location !\n");
        printf("============================\n"); 

    } else {
        fprintf(stderr, "Error: Invalid mode\n");
        exit(EXIT_FAILURE);
    }
}






