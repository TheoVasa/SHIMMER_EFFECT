#ifndef SHIMMER_H
#define SHIMMER_H
#include "schroeder_reverberator.h"
#include "gs_pitchshift.h"
#include "butterworth.h"
#include "delay_line.h"
#include "utils.h"
//-----------------------------------------------------------------
//macros declarations
//-----------------------------------------------------------------



//-----------------------------------------------------------------
//structures delcarations
//-----------------------------------------------------------------
/**
 * @brief represent a Shimmer effect
 * 
 * @param reverberator The Schroeder reverberator.
 * 
 * @param pitch_shifter The pitch shifter.
 * 
 * @param lowpass The low-pass filter.
 * 
 * @param highpass The high-pass filter.
 * 
 * @param delay_line The delay line.
 * 
 * @param parameters The parameters of the shimmer effect.
 * 
*/
typedef struct {
    Schroeder_reverberator* reverberator;
    GS_pitchshift *pitch_shifter;
    Butterworth *lowpass;
    Butterworth *highpass;
    DelayLine *delay_line;
    Parameters *parameters;
} Shimmer;

//-----------------------------------------------------------------
//function declarations
//-----------------------------------------------------------------

/**
 * @brief Create the Shimmer
 * 
 * @param parameters The user parameters of the shimmer effect.
 * 
 * @param grain_size The grain size of the pitch shifter.
 * 
 * @param fade The fade of the pitch shifter.
 * 
 * @return The dynamic allocated pitch shifter
 * 
*/
Shimmer* init_shimmer(Parameters *parameters, int grain_size, double fade);

/**
 * @brief Reset the shimmer internal buffers
 * 
 * @param shimmer The shimmer.
 * 
 * @return void
*/
void reset_shimmer(Shimmer* shimmer);

/**
 * @brief Apply the shimmer effect to a signal
 * 
 * @param shimmer The shimmer.
 * 
 * @param x The input buffer.
 * 
 * @param y The output buffer.
 * 
 * @return void
*/
void apply_shimmer(Shimmer* shimmer, double* x, double* y);

/**
 * @brief Free the memory allocated by the shimmer effect
 * 
 * @param shimmer The shimmer.
 * 
 * @return void
*/
void free_shimmer(Shimmer* shimmer);

#endif