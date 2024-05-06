#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"

Parameters getUserParameters(void) {
    Parameters params; 
    char mode[20];

    //ask for the mode 
    printf("Enter mode (real-time or play-back): ");
    scanf("%s", mode);
    while (strcmp(mode, "real-time") != 0 && strcmp(mode, "play-back") != 0) {
        printf("Invalid mode. Please enter real-time or play-back :");
        scanf("%s", mode);
    }
    //ask if we want to use the default parameters
    printf("Do you want to use default parameters for the shimmer ? [y/n]\n"); 
    char answer[10];
    scanf("%s", answer);
    while(strcmp(answer, "y") != 0 && strcmp(answer, "n") != 0){
        printf("Invalid answer. Please enter y or n :");
        scanf("%s", answer);
    }
    if(strcmp(answer, "y") == 0){
        
        params = DEFAULT_PARAMS; 
        strcpy(params.mode, mode);

        goto recap;
    }

    //initialize the parameters according to user 
    strcpy(params.mode, mode);

    //the mix
    printf("Enter mix value (between 0 and 1): ");
    scanf("%lf", &params.mix);
    while (params.mix < 0 || params.mix > 1) {
        printf("Invalid mix value. Please enter a value between 0 and 1: ");
        scanf("%lf", &params.mix);
    }
    //the highcut
    printf("Enter highcut value (between 1 and 20000): ");
    scanf("%lf", &params.highcut);
    while (params.highcut < 1 || params.highcut > 20000) {
        printf("Invalid highcut value. Please enter a value between 1 and 20000: ");
        scanf("%lf", &params.highcut);
    }
    //the lowcut
    printf("Enter lowcut value (between 1 and 20000): ");
    scanf("%lf", &params.lowcut);
    while (params.lowcut < 1 || params.lowcut > 20000) {
        printf("Invalid highcut value. Please enter a value between 1 and 20000: ");
        scanf("%lf", &params.lowcut);
    }
    //the feedback
    printf("Enter feedback value (between 0 and 0.1): ");
    scanf("%lf", &params.feedback);
    while (params.feedback < 0 || params.feedback > 0.1) {
        printf("Invalid feedback value. Please enter a value between 0 and 0.1: ");
        scanf("%lf", &params.feedback);
    }
    //the shift
    printf("Enter shift value (between -12 and 12): ");
    scanf("%lf", &params.shift);
    while (params.shift < -12 || params.shift > 12) {
        printf("Invalid shift value. Please enter a value between -12 and 12: ");
        scanf("%lf", &params.shift);
    }
    //the size
    printf("Enter size value (between 0 and 1): ");
    scanf("%lf", &params.size);
    while (params.size < 0 || params.size > 1) {
        printf("Invalid size value. Please enter a value between 0 and 1: ");
        scanf("%lf", &params.size);
    }
    //the diffusion
    printf("Enter diffusion value (between 0 and 10): ");
    scanf("%lf", &params.diffusion);
    while (params.diffusion < 0 || params.diffusion > 10) {
        printf("Invalid diffusion value. Please enter a value between 0 and 10: ");
        scanf("%lf", &params.diffusion);
    }

    //recap of the parameters
    recap : 
    //clean the output
    //system("cls"); 
    printf("Mode: %s\n", params.mode);
    printf("========= PARAMETERS =======\n"); 
    printf("Mix: %.2f\n", params.mix);
    printf("Shift: %.2f\n", params.shift); 
    printf("Feedback: %.2f\n", params.feedback);
    printf("Size: %.2f\n", params.size);
    printf("Diffusion: %.2f\n", params.diffusion);
    printf("Lowcut: %.2f\n", params.lowcut);
    printf("Highcut: %.2f\n", params.highcut);

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
            win[i] = (double)i/(double)fade_length; 
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

int is_equal(data_t x, data_t y){
    return fabs(x-y) < 1e-6*fabsl(y);
}







