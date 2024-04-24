#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

Parameters getUserParameters() {
    Parameters params;
    //mode
    printf("Enter mode (real-time, play-back, or record): ");
    scanf("%s", params.mode);
    while (strcmp(params.mode, "real-time") != 0 && strcmp(params.mode, "play-back") != 0 && strcmp(params.mode, "record") != 0) {
        printf("Invalid mode. Please enter real-time, play-back, or record: ");
        scanf("%s", params.mode);
    }
    //mix
    printf("Enter mix value (between 0 and 1): ");
    scanf("%f", &params.mix);
    while (params.mix < 0 || params.mix > 1) {
        printf("Invalid mix value. Please enter a value between 0 and 1: ");
        scanf("%f", &params.mix);
    }
    //highcut
    printf("Enter highcut value (between 1 and 20000): ");
    scanf("%f", &params.highcut);
    while (params.highcut < 1 || params.highcut > 20000) {
        printf("Invalid highcut value. Please enter a value between 1 and 20000: ");
        scanf("%f", &params.highcut);
    }
    //lowcut
    printf("Enter lowcut value (between 1 and 20000): ");
    scanf("%f", &params.lowcut);
    while (params.lowcut < 1 || params.lowcut > 20000) {
        printf("Invalid highcut value. Please enter a value between 1 and 20000: ");
        scanf("%f", &params.lowcut);
    }
    //feedback
    printf("Enter feedback value (between 0 and 0.1): ");
    scanf("%f", &params.feedback);
    while (params.feedback < 0 || params.feedback > 0.1) {
        printf("Invalid feedback value. Please enter a value between 0 and 0.1: ");
        scanf("%f", &params.feedback);
    }
    //shift
    printf("Enter shift value (between -12 and 12): ");
    scanf("%f", &params.shift);
    while (params.shift < -12 || params.shift > 12) {
        printf("Invalid shift value. Please enter a value between -12 and 12: ");
        scanf("%f", &params.shift);
    }
    //size
    printf("Enter size value (between 0 and 1): ");
    scanf("%f", &params.size);
    while (params.size < 0 || params.size > 1) {
        printf("Invalid size value. Please enter a value between 0 and 1: ");
        scanf("%f", &params.size);
    }
    //diffusion
    printf("Enter diffusion value (between 0 and 10): ");
    scanf("%f", &params.diffusion);
    while (params.diffusion < 0 || params.diffusion > 10) {
        printf("Invalid diffusion value. Please enter a value between 0 and 10: ");
        scanf("%f", &params.diffusion);
    }

    //clean the output
    system("cls"); 

    //recap of the parameters
    printf("Mode: %s\n", params.mode);
    printf("=== PARAMETERS ===\n"); 
    printf("Mix: %.2f\n", params.mix);
    printf("Shift: %.2f\n", params.shift); 
    printf("Feedback: %.2f\n", params.feedback);
    printf("Size: %.2f\n", params.size);
    printf("Diffusion: %.2f\n", params.diffusion);
    printf("Lowcut: %.2f\n", params.lowcut);
    printf("Highcut: %.2f\n", params.highcut);
    printf("==================\n"); 

    return params;
}

int time_to_samples(int time) {
    return (int)((time * SAMPLE_RATE) / 1000);
}






