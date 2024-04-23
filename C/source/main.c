#include <stdio.h>
#include <math.h>
#include "portaudio.h"

#define SAMPLE_RATE 44100
#define NUM_SECONDS 5
#define NUM_CHANNELS 1

typedef struct {
    double phase;
    double frequency;
} SineWaveData;

int sineWaveCallback(const void* inputBuffer, void* outputBuffer,
                     unsigned long framesPerBuffer,
                     const PaStreamCallbackTimeInfo* timeInfo,
                     PaStreamCallbackFlags statusFlags,
                     void* userData)
{
    float* out = (float*)outputBuffer;
    SineWaveData* data = (SineWaveData*)userData;

    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        double sample = sin(data->phase * 2 * 3.14);
        *out++ = (float)sample;
        data->phase += data->frequency / SAMPLE_RATE;
    }

    return paContinue;
}

int main()
{
    Pa_Initialize();

    SineWaveData data;
    data.phase = 0.0;
    data.frequency = 440.0;

    PaStream* stream;
    Pa_OpenDefaultStream(&stream, 0, NUM_CHANNELS, paFloat32,
                         SAMPLE_RATE, paFramesPerBufferUnspecified,
                         sineWaveCallback, &data);

    Pa_StartStream(stream);
    Pa_Sleep(NUM_SECONDS * 1000);
    Pa_StopStream(stream);
    Pa_CloseStream(stream);

    Pa_Terminate();

    return 0;
}