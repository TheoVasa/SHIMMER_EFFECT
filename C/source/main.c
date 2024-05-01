#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>

#include "utils.h"
#include "shimmer.h"
#include "audio.h"

int main(int argc, char* argv[]) {
    //get user parameters and init the shimmer
    Parameters params = getUserParameters();
    Shimmer* shimmer = init_shimmer(&params);

    //start the audio process 
    audio_process(params.mode, shimmer);

    //free the allocated memory for the shimmer 
    free_shimmer(shimmer);
    return 0;
}
