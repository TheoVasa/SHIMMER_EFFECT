#ifndef SCHROEDER_REVERBERATOR_H
#define SCHROEDER_REVERBERATOR_H

#include "iir.h"
//-----------------------------------------------------------------
//macros declarations
//-----------------------------------------------------------------


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