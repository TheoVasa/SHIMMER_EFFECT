#ifndef SCHROEDER_REVERBERATOR_H
#define SCHROEDER_REVERBERATOR_H

#include "iir.h"
//-----------------------------------------------------------------
//macros declarations
//-----------------------------------------------------------------

//template for the Schroeder reverberator filters parameters
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
//the wet ratio
#define WET_REVERB 0.4

//-----------------------------------------------------------------
//structures delcarations
//-----------------------------------------------------------------
/**
 * @brief represent a Schroeder reverberator
 * 
 * @param allpasses The all-pass filters.
 * 
 * @param combs The comb filters.
 * 
 * @param n_ap The number of all-pass filters.
 * 
 * @param n_c The number of comb filters.
 * 
 * @param wet The wet ratio of the reverberator.
*/
typedef struct {
    IIR** allpasses; 
    IIR** combs;
    int n_ap; 
    int n_c;
    double wet; 
} Schroeder_reverberator;

//-----------------------------------------------------------------
//function declarations
//-----------------------------------------------------------------

/**
 * @brief Create a Schroeder reverberator
 * 
 * @param size The size parameter of the reverberator.
 * 
 * @param diffusion The diffusion parameter of the reverberator.
 * 
 * @return The dynamic allocated Schroeder reverberator
 * 
*/
Schroeder_reverberator *init_schroeder(double size, double diffusion);

/**
 * @brief Reset the Schroeder reverberator internal buffers
 * 
 * @param schroeder The Schroeder reverberator.
 * 
 * @return void
*/
void reset_schroeder(Schroeder_reverberator* schroeder);

/**
 * @brief Apply the Schroeder reverberator to a signal
 * 
 * @param schroeder The Schroeder reverberator.
 * 
 * @param x The input buffer.
 * 
 * @param y The output buffer.
 * 
 * @param buffer_size The size of the buffer.
 * 
 * @return void
*/
void filter_schroeder(Schroeder_reverberator* schroeder, double* x, double* y, int buffer_size); 

/**
 * @brief Free the memory allocated by the Schroeder reverberator
 * 
 * @param schroeder The Schroeder reverberator.
 * 
 * @return void
*/
void free_schroeder(Schroeder_reverberator* schroeder);

/**
 * @brief generate an all-pass filter given a gain and a delay
 * 
 * @param gain The gain of the all-pass filter.
 * 
 * @param delay The delay of the all-pass filter.
 * 
 * @return The dynamic allocated IIR filter
*/
IIR *generate_allpass(double gain, double delay);

/**
 * @brief generate a comb filter given a gain and a delay
 * 
 * @param gain The gain of the comb filter.
 * 
 * @param delay The delay of the comb filter.
 * 
 * @return The dynamic allocated IIR filter
*/
IIR *generate_comb(double gain, double delay);

#endif