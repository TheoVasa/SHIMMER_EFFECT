#ifndef UTILS_H
#define UTILS_H
//-----------------------------------------------------------------
//macros declarations
//-----------------------------------------------------------------

//audio parameters
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER (64)

//template for the Schroeder reverberator
//filters parameters
#define PRIMES {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113}
#define PRIME_COUNT 30
#define BASIC_GAIN_AP 0.7
#define BASIC_GAINS_C {0.773, 0.802, 0.753, 0.733}
#define BASIC_DELAYS_C {113.2, 127.3, 153.5, 151.7}
#define MAX_SHIFT_GAIN_C 0.1
#define MAX_SHIFT_DELAY_C 100.0
#define MIN_AP 3
#define MAX_AP 9
#define N_C 4

//template for the pitch shifter parameters
#define GRAIN_SIZE 1024
#define FADE 0.4

//the lowpass and highpass filters parameters
#define LOWPASS 0
#define HIGHPASS 1
#define ORDER 4

//template for the internal shimmer parameters
#define WET_REVERB 0.4 //the dry/wet ratio of the reverb
#define PITCH_RATIO 0.2 //the dry/wet ratio of the pitcher

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
void resample(double* x, double* y, int x_size, int y_size, double factor);

/**
 * @brief generate the wanted shift factor given the pitch shift in semitones
 * 
 * @param shift The pitch shift in semitones
 * 
 * @return The shift factor
*/
double shift_factor(int shift); 

#endif


