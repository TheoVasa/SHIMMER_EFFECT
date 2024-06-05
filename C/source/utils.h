#ifndef UTILS_H
#define UTILS_H

#include "portaudio.h"
//-----------------------------------------------------------------
//macros declarations
//-----------------------------------------------------------------

/**
 * project parameters
*/
#define PATH_TO_AUDIO "./audio"
//shimmer parameters template (used to run without specifying all the parameters)
#define DEFAULT_PARAMS (Parameters){.mode="",.lowcut=100,.highcut=10000,.mix=0.4,.feedback=0.5,.shift=12.0,.size=0.5,.diffusion=0.5,.depth=0.5,.rate=0.5}

/**
 * shimmer parameters
*/
//the dry/wet ratio of the reverberator
#define WET_REVERB 0.7
//the dry/wet ratio of the pitcher (old design)
#define PITCH_RATIO 1
//the minimum value of the feedback
#define MIN_FEEDBACK 0.0
//the maximum value of the feedback
#define MAX_FEEDBACK 0.1
//the minimum value of the size
#define MIN_SIZE 0.0
//the maximum value of the size
#define MAX_SIZE 1.0
//the minimum value of the diffusion
#define MIN_DIFFUSION 0.0
//the maximum value of the diffusion
#define MAX_DIFFUSION 10.0
//the minimum value of the depth
#define MIN_DEPTH 0.0
//the maximum value of the depth
#define MAX_DEPTH 0.01
//the minimum value of the rate
#define MIN_RATE 10.0
//the maximum value of the rate
#define MAX_RATE 20.0

/**
 * audio parameters
 */
//the sample rate
#define SAMPLE_RATE 44100
//the buffer size (will be used in play-back mode)
#define BUFFER_SIZE (128)
//the maximum buffer size 
#define MAX_BUFFER_SIZE (2048)
//stereo offset in ms(to simulate stereo effect)
#define STEREO_OFFSET (5.0)
//sample type 
#define data_t float

/**
 * template for the Schroeder reverberator
*/
//filters parameters
#define PRIMES {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113}
#define PRIME_COUNT 30
//basic gain parameter for an all-pass filter
#define BASIC_GAIN_AP 0.7
//basic gains for 4 combs filters in parallel
#define BASIC_GAINS_C {0.773, 0.802, 0.753, 0.733, 0.801, 0.753}
//basic delays for 4 combs filters in parallel
#define BASIC_DELAYS_C {113.2, 127.3, 153.5, 151.7, 251.3, 222.3}
//maximum shift gain for the comb filters (shift from the basic gain with max size parameter)
#define MAX_SHIFT_GAIN_C 0.1
//maximum shift gain for the comb filters (shift from the basic gain with max size parameter)
#define MAX_SHIFT_DELAY_C 100.0
//minimum number of all passes filters (minimmum diffusion)
#define MIN_AP 3
//maximum number of all pass filters (maximum diffusion)
#define MAX_AP 8
//number of comb filters
#define N_C 6

/**
 * template for the pitch shifter
 */
//size of the grains
#define GRAIN_SIZE (time_to_samples(75))
//fade porcentage of the tapering window
#define FADE 0.5
//the jump we have to make to correctly overlap our grains
#define JUMP (int)(GRAIN_SIZE - GRAIN_SIZE*FADE/2.0)
//the number of overlaped grains
#define OVERLAP (GRAIN_SIZE - JUMP)
//the maximum size of the internal input/output buffers
#define MAX_LEN_BUF (GRAIN_SIZE + MAX_BUFFER_SIZE)

/**
 * the lowpass and highpass filters parameters
 */
//reprensent LOWPASS enum
#define LOWPASS 0
//reprensent HIGHPASS enum
#define HIGHPASS 1
//order of the butterworth filter
#define ORDER 5

/**
 * Useful macros
*/
//maximum of two numbers
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define M_PI 3.14159265358979323846
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
 * @param depth The depth of the modulations on the filters
 * 
 * @param rate The rate of the modulations on the filters
 * 
*/
typedef struct {
    char mode[20];
    double lowcut;
    double highcut;
    double mix;
    double feedback;
    double shift;
    double size;
    double diffusion;
    double depth;
    double rate;
} Parameters;


//-----------------------------------------------------------------
//function declarations
//-----------------------------------------------------------------

/**
 * @brief Get the parameters of the shimmer effect from the user
 * 
 * @param void
 * 
 * @return The parameters of the shimmer effect
*/
Parameters getUserParameters(void);

/**
 * @brief compute the number of samples corresponding to a given time in ms
 * 
 * @param time The time in ms
 * 
 * @return The number of samples
*/
int time_to_samples(int time);


/**
 * @brief generate a linear tapering window for the pitch shifter
 * 
 * @param size The size of the window
 * 
 * @param fade The fade of the window
 * 
 * @return The window
*/
double *generate_win(int size, double fade); 


/**
 * @brief resample a signal
 * 
 * @param x The input signal
 * 
 * @param y The output signal
 * 
 * @param x_size The size of the input signal
 * 
 * @param y_size The size of the output signal
 * 
 * @param factor The resampling factor
 * 
 * @return void
 * 
 * 
*/
void resample(data_t* x, data_t* y, int x_size, int y_size, double factor);

/**
 * @brief generate the wanted shift factor given the pitch shift in semitones
 * 
 * @param shift The pitch shift in semitones
 * 
 * @return The shift factor
*/
double pitch_factor(int shift); 

/**
 * @brief rescale all the parameters of the shimmer effect
 * 
 * @param params The parameters of the shimmer effect
 * 
 * @return void
*/
void rescale_parameters(Parameters *params);

#endif


