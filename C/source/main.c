#include <stdio.h>
#include "utils.h"
#include "shimmer.h"
#include "audio.h"

int main(int argc, char* argv[]) {
    start : 
    //get user parameters and init the shimmer
    Parameters params = getUserParameters();
    Shimmer* shimmer = init_shimmer(&params);

    //start the audio process 
    audio_process(params.mode, shimmer);

    printf("Press <q> then <enter> to quit or any other key then <enter> to relaunch the program\n"); 
    char choice;
    scanf(" %c", &choice);
    if (choice == 'q') {
        return 0;
    } else {
        goto start;
    }
}
