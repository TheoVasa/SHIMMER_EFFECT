#ifndef AUDIO_H
#define AUDIO_H

#include "utils.h"
#include "shimmer.h"
#include "portaudio.h"


//-----------------------------------------------------------------
//static attributes
//-----------------------------------------------------------------
//The shimmer that will be used in the callback
static Shimmer* shimmer;

//-----------------------------------------------------------------
//structures delcarations
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//function declarations
//-----------------------------------------------------------------

/**
 * @brief The callback function that will be used by the portaudio library to perform real-time audio processes (take input from microphone, process using shimmer and write to output buffer)
 * 
 * @param inputBuffer The input buffer
 * 
 * @param outputBuffer The output buffer
 * 
 * @param framesPerBuffer The number of frames per buffer
 * 
 * @param timeInfo The time information
 * 
 * @param statusFlags The status flags
 * 
 * @param userData The user data
 * 
 * @return The result of the callback
*/
int rt_callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

/**
 * @brief The callback function that will be used by the portaudio library to record audio from the microphone
 * 
 * @param inputBuffer The input buffer
 * 
 * @param outputBuffer The output buffer
 * 
 * @param framesPerBuffer The number of frames per buffer
 * 
 * @param timeInfo The time information
 * 
 * @param statusFlags The status flags
 * 
 * @param userData The user data
 * 
 * @return The result of the callback
*/
int record_callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);


/**
 * @brief The callback function that will be used by the portaudio library to playback processed audio from a file
 * 
 * @param inputBuffer The input buffer
 * 
 * @param outputBuffer The output buffer
 * 
 * @param framesPerBuffer The number of frames per buffer
 * 
 * @param timeInfo The time information
 * 
 * @param statusFlags The status flags
 * 
 * @param userData The user data
 * 
 * @return The result of the callback
*/
int playback_callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

/**
 * @brief Process the audio signal given the mode (record, playback, real-time)
 * 
 * @param mode The mode of the audio processing
 * 
 * @param sh The shimmer effect
 * 
 * @return void
*/
void audio_process(const char *mode, Shimmer *sh); 

#endif


