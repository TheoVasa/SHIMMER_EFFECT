#ifndef SHIMMER_H
#define SHIMMER_H
#include "schroeder_reverberator.h"
#include "gs_pitchshift.h"
#include "butterworth.h"
#include "delay_line.h"
#include "utils.h"

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
 * @param delay_line1 The first delay line.
 * 
 * @param delay_line2 The second delay line.
 * 
 * @param parameters The parameters of the shimmer effect.
 * 
 * @param feedback_buf The internal output buffer for the feedback loop
 * 
*/
typedef struct {
    Schroeder_reverberator* reverberator;
    GS_pitchshift *pitch_shifter;
    Butterworth *lowpass;
    Butterworth *highpass;
    DelayLine *delay_line1;
    DelayLine *delay_line2;
    Parameters *parameters;
    data_t* feedback_buf;
} Shimmer;

//-----------------------------------------------------------------
//function declarations
//-----------------------------------------------------------------

/**
 * @brief Create the Shimmer
 * 
 * @param parameters The user parameters of the shimmer effect.
 * 
 * @return The dynamic allocated pitch shifter
 * 
*/
Shimmer* init_shimmer(Parameters *parameters);

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
 * @param buffer_size The size of the buffer.
 * 
 * @return void
*/
void apply_shimmer(Shimmer* shimmer, data_t* x, data_t* y, int buffer_size);

/**
 * @brief Free the memory allocated by the shimmer effect
 * 
 * @param shimmer The shimmer.
 * 
 * @return void
*/
void free_shimmer(Shimmer* shimmer);

#endif