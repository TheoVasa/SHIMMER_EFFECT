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
 * @brief represent the parameters of the shimmer effect
 * 
 * @param mode The mode of the shimmer effect.
 * 
 * @param lowcut The lowcut frequency.
 * 
 * @param highcut The highcut frequency.
 * 
 * @param mix The mix of the shimmer effect.
 * 
 * @param feedback The feedback of the shimmer effect.
 * 
 * @param shift The pitch shift of the shimmer effect.
 * 
 * @param size The size of the shimmer effect.
 * 
 * @param diffusion The diffusion of the shimmer effect.
 * 
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


