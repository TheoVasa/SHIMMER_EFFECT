#ifndef BUTTERWORTH_H
#define BUTTERWORTH_H

#include "iir.h"
//-----------------------------------------------------------------
//macros declarations
//-----------------------------------------------------------------

#define LOWPASS 0
#define HIGHPASS 1
#define ORDER 4

//-----------------------------------------------------------------
//structures delcarations
//-----------------------------------------------------------------

/**
 * @brief represent an butterworth filter
 * 
 * @param iir The IIR filter.
 * 
 * 
 * @param type The type of the butterworth filter (0 for low-pass, 1 for high-pass).
*/
typedef struct butterworth {
    IIR* iir;
    int type;
} Butterworth;

//-----------------------------------------------------------------
//function declarations
//-----------------------------------------------------------------

/**
 * @brief Create a butterworth filter
 * 
 * @param type The type of the butterworth filter (0 for low-pass, 1 for high-pass).
 * 
 * @param fc The cut-off frequency of the butterworth filter.
 * 
 * @return The dynamic allocated butterworth filter
*/
Butterworth *init_butterworth(int type, double fc);

/**
 * @brief Reset the butterworth filter internal buffers
 * 
 * @param butterworth The butterworth filter.
 * 
 * @return void
*/
void reset_butterworth(Butterworth* butterworth);

/**
 * @brief Filter a signal with a butterworth filter
 * 
 * @param butterworth The butterworth filter.
 * 
 * @param x The input buffer.
 * 
 * @param y The output buffer.
 * 
 * @param buffer_size The size of the buffer.
 * 
 * @return void
*/
void filter_butterworth(Butterworth* butterworth, double* x, double* y, int buffer_size);

/**
 * @brief Free the memory allocated by the butterworth filter
 * 
 * @param butterworth The butterworth filter.
 * 
 * @return void
*/
void free_butterworth(Butterworth* butterworth);

#endif