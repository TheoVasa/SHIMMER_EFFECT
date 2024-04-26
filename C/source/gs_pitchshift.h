#ifndef GS_PITCHSHIFT_H
#define GS_PITCHSHIFT_H

#include "utils.h"
//-----------------------------------------------------------------
//structures delcarations
//-----------------------------------------------------------------
/**
 * @brief represent a pitch shifter
 * 
 * @param shift_factor The pitch shift factor.
 * 
 * @param input_size The input number of sample to make GRAIN_SIZE grains
 *   
 * @param win The window.
 * 
 * @param input_buf The internal input buffer.
 * 
 * @param len_input_buf The length of the input buffer.
 * 
 * @param output_buf The internal output buffer.
 * 
 * @param len_output_buf The length of the output buffer.
 * 
 * @param last_grain The last grain stored
 * 
*/
typedef struct {
    double shift_factor;
    int input_size; 
    double* win;
    data_t* input_buf;
    int len_input_buf; 
    data_t* output_buf;
    int len_output_buf; 
    data_t* last_grain;
} GS_pitchshift;

//-----------------------------------------------------------------
//function declarations
//-----------------------------------------------------------------

/**
 * @brief Create a pitch shifter
 * 
 * @param shift_factor The pitch shift factor.
 * 
 * @return The dynamic allocated pitch shifter
*/
GS_pitchshift* init_gs_pitchshift(int shift_factor);

/**
 * @brief Reset the pitch shifter internal buffers
 * 
 * @param pitch_shifter The pitch shifter.
*/
void reset_gs_pitchshift(GS_pitchshift* pitch_shifter);

/**
 * @brief Apply the pitch shifter to a signal
 * 
 * @param pitch_shifter The pitch shifter.
 * 
 * @param x The input buffer.
 * 
 * @param y The output buffer.
 * 
 * @param buffer_size The size of the buffer.
*/
void filter_gs_pitchshift(GS_pitchshift* pitch_shifter, data_t* x, data_t* y, int buffer_size);

/**
 * @brief Free the memory allocated by the pitch shifter
 * 
 * @param pitch_shifter The pitch shifter.
 * 
 * @return void
*/
void free_gs_pitchshift(GS_pitchshift* pitch_shifter);


#endif