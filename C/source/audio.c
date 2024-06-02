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
    //get the input samples corresponding to the chosen channel
    data_t in_channel[framesPerBuffer]; 
    for (int i = 0; i < framesPerBuffer; ++i) {
        in_channel[i] = in[data->numInputChannels*i + data->channel]; 
    }
    if(data->numOutputChannels == 2){
        data_t left[framesPerBuffer];
        data_t right[framesPerBuffer];
        if(data->bypass){
            //copy the input to the output
            for (int i = 0; i < framesPerBuffer; ++i){
                left[i] = in_channel[i];
            }
        } else {
        apply_shimmer(data->shimmer, in_channel, left, framesPerBuffer);
        }
        //delay right channel to emulate stereo output
        apply_delay_line(data->delayLine, left, right, framesPerBuffer);
        //interleave the left and right channels
        for (int i = 0; i < framesPerBuffer; ++i) {
            out[2*i] = left[i];
            out[2*i + 1] = right[i];
        }
    } else {
        //process the input buffer using the shimmer effect
        if(data->bypass){
            //copy the input to the output
           for (int i = 0; i < framesPerBuffer; ++i){
                out[i] = in_channel[i];
           }
           
        } else {
        apply_shimmer(data->shimmer, in_channel, out, framesPerBuffer);
        }
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
    data.bypass = 0;

    //ask the user the buffer size : a custom one (smalller than MAX_BUFFER_SIZE) or let portaudio decide
    printf("Enter the buffer size or type 'auto' to let PortAudio decide: ");
    char bs[10];
    int bufferSize;
    scanf("%s", bs);
    if(strcmp(bs, "auto") == 0){
        bufferSize = 0;
    } else {
        bufferSize = atoi(bs);
        while(bufferSize <= 0 || bufferSize > MAX_BUFFER_SIZE){
            if(bufferSize <= 0){
                printf("Invalid buffer size. Please enter a positive integer: ");
            } else if(bufferSize > MAX_BUFFER_SIZE){
                printf("Invalid buffer size. Please enter a value smaller than %d: ", MAX_BUFFER_SIZE);
            } 
            scanf("%s", bs);
            bufferSize = atoi(bs);
        }
    }

    //check if the host API is asio
    int asio; 
    const PaHostApiInfo *hostApiInfo = Pa_GetHostApiInfo(Pa_GetDefaultHostApi());
    if (strcmp(hostApiInfo->name, "ASIO") == 0) {
        asio = 1;
    } else {
        asio = 0;
    }

    //-----------------------------------------------------------------
    //input
    PaStreamParameters inputParameters, outputParameters;
    inputParameters.device = Pa_GetDefaultInputDevice();

    //test if the input device is valid
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr, "Error: No default input device, if you are using ASIO, make sure you have an ASIO interface connected.\n");
        exit(EXIT_FAILURE);
    }
    

    //check if the device as more than one channel, if it's the case, ask the user to choose the channel
    const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(inputParameters.device);
    data.numInputChannels = deviceInfo->maxInputChannels;
    if (deviceInfo->maxInputChannels > 1 ) {
        printf("Your device has more than one channel and the shimmer only support mono inputs.\nPlease enter the channel number you want to use [%d - %d]: ", 0, deviceInfo->maxInputChannels - 1);
        int channel;
        scanf("%d", &channel);
        while (channel < 0 || channel >= deviceInfo->maxInputChannels) {
            fprintf(stderr, "Error: Invalid channel number\n");
            scanf("%d", &channel);}
        data.channel = channel; 
        printf("\n"); 
    }
    else {
        //if one channel
        data.channel = 0;
    }

    inputParameters.channelCount = deviceInfo->maxInputChannels;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = deviceInfo->defaultLowInputLatency; 
    inputParameters.hostApiSpecificStreamInfo = NULL;

    //-----------------------------------------------------------------
    //output
    outputParameters.device = Pa_GetDefaultOutputDevice();
    //test if the output device is valid
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr, "Error: No default output device, if you are using ASIO, make sure you have an ASIO interface connected.\n");
        exit(EXIT_FAILURE);
    }
    //ask the user if he wants stereo or mono output
    int num_channel_out = 0;
    printf("Do you want stereo output ? [y/n]");
    char answer[10];
    scanf("%s", answer);
    while(strcmp(answer, "y") != 0 && strcmp(answer, "n") != 0){
        printf("Invalid answer. Please enter y or n :");
        scanf("%s", answer);
    }
    printf("\n");
    if(strcmp(answer, "y") == 0){
        num_channel_out = 2;
    } else if (strcmp(answer, "n") == 0){
        num_channel_out = 1;
        //check the max number of channel of the output device
        const PaDeviceInfo *outputDeviceInfo = Pa_GetDeviceInfo(outputParameters.device);
        if(outputDeviceInfo->maxOutputChannels > 1){
            printf("You've chosen mono output but your device has multiple output channels. You may ear the output only on the left or right side of the speaker.\n");
        } 
    }
    data.numOutputChannels = num_channel_out;
    const PaDeviceInfo *outputDeviceInfo = Pa_GetDeviceInfo(outputParameters.device);
    outputParameters.channelCount = num_channel_out; 
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = outputDeviceInfo->defaultLowOutputLatency; 
    outputParameters.hostApiSpecificStreamInfo = NULL;

    //-----------------------------------------------------------------
    //print all the informations 
    printf("----------------------------\n");
    printf("Starting real-time audio processing\n\n");
    //host API 
    printf("Host API: %s \n", hostApiInfo->name);
    //print the input device
    printf("Input device: %s \n", deviceInfo->name);
    //print the output device
    printf("Output device: %s \n", outputDeviceInfo->name);
    //print the input channel
    if(deviceInfo->maxInputChannels > 1) printf("Chosen input channel: %d \n", data.channel);
    //print the output channel
    if(num_channel_out == 1){
        printf("Output mode: Mono\n");
    } else if(num_channel_out == 2){
        printf("Output mode: Stereo\n");
    }
    //print the buffer size
    if(bufferSize == 0){
        printf("Buffer size: Auto\n");
    } else {
        printf("Buffer size: %d\n", bufferSize);
    }
    printf("\n/!/ If you are using a microphone, wear headphones to avoid feedback and larsen /!/\n\n");

    //-----------------------------------------------------------------
    // Open the audio stream
    PaStream *stream;
    err = Pa_OpenStream(&stream, &inputParameters, &outputParameters, SAMPLE_RATE, bufferSize, paNoFlag, &rt_callback, &data);
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
    //-----------------------------------------------------------------
    
    // Wait for the user to type stop or bypass
    printf("Press 'stop' then <enter> to quit\n");
    printf("Press 'bypass' then <enter> to bypass or reactivate the effect\n"); 
    printf("----------------------------\n");
    char cmd[10];
    scanf("%s", cmd);
    while(strcmp(cmd, "stop") != 0){
        if(strcmp(cmd, "bypass") == 0){
            if(data.bypass){
                data.bypass = 0;
                printf("Effect reactivated\n");
            } else {
                data.bypass = 1;
                printf("Effect bypassed\n");
            }
            scanf("%s", cmd);
        } else {
            printf("Invalid command\n");
        }
    }
    //-----------------------------------------------------------------
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
        int block_size = MAX_BUFFER_SIZE;
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






