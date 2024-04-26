#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"

Parameters getUserParameters(void) {
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

double *generate_win(int size, double fade){
    double *win = (double*)malloc(size*sizeof(double));
    //compute the fade length 
    int fade_length = (int)(size*fade/2.0);
    //compute the window 
    for(int i=0; i<size; i++){
        //fade-in
        if(i<fade_length){
            win[i] = (double)i/(double)size; 
        }
        //fade-out
        else if(i>size-fade_length){
            win[i] = 1.0 - win[i-(size-fade_length)];
        }
        //no fade
        else {
            win[i] = 1.0;
        }
    }
    return win;
}


void resample(data_t* x, data_t* y, int x_size, int y_size, double factor){
    //resample the signal using first order interpolation
    for(int i = 0; i < y_size; i++){
        double s = i*factor;
        int n = floor(s); 
        data_t x1 = 0; 
        data_t x2 = 0; 
        if(n < x_size) {
            x1 = x[n];
        } 
        if(n+1 < x_size){
            x2 = x[n+1];
        }
        y[i] = (data_t)((1-s+n)*x1 + (s-n)*(x2));
    }
}


double pitch_factor(int shift){
    return pow(2, shift/12.0);
}






