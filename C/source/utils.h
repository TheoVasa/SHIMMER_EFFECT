#ifndef UTILS_H
#define UTILS_H
//-----------------------------------------------------------------
//macros declarations
//-----------------------------------------------------------------

#define SAMPLE_RATE 44100


//-----------------------------------------------------------------
//structures delcarations
//-----------------------------------------------------------------

/**
 * Structure to store the parameters of the shimmer effect
*/
typedef struct {
    char mode[20];
    float lowcut;
    float highcut;
    float mix;
    float feedback;
    float shift;
    float size;
    float diffusion;
} Parameters;


//-----------------------------------------------------------------
//function declarations
//-----------------------------------------------------------------

/**
 * @brief Get the parameters of the shimmer effect from the user
 * 
 * @return The parameters of the shimmer effect
*/
Parameters getUserParameters();

/**
 * @brief compute the number of samples corresponding to a given time in ms
 * 
 * @param time The time in ms
 * 
 * @return The number of samples
*/
int time_to_samples(int time);

#endif


