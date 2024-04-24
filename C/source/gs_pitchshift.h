#ifndef GS_PITCHSHIFT_H
#define GS_PITCHSHIFT_H
//-----------------------------------------------------------------
//macros declarations
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//structures delcarations
//-----------------------------------------------------------------
/**
 * @brief represent a pitch shifter
 * 
 * @param shift_factor The pitch shift factor.
 * 
 * @param grain_size The grain size.
 * 
 * @param fade The fade.
 * 
 * @param win The window.
 * 
 * @param input_buf The internal input buffer.
 * 
 * @param output_buf The internal output buffer.
 * 
*/
typedef struct {
    int shift_factor;
    int grain_size; 
    double fade; 
    double* win;
    double* input_buf;
    double* output_buf;
} GS_pitchshift;

//-----------------------------------------------------------------
//function declarations
//-----------------------------------------------------------------

/**
 * @brief Create a pitch shifter
 * 
 * @param shift_factor The pitch shift factor.
 * 
 * @param grain_size The grain size.
 * 
 * @param fade The fade.
 * 
 * @return The dynamic allocated pitch shifter
*/
GS_pitchshift* init_gs_pitchshift(int shift_factor, int grain_size, double fade);

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
*/
void filter_gs_pitchshift(GS_pitchshift* pitch_shifter, double* x, double* y);

/**
 * @brief Free the memory allocated by the pitch shifter
 * 
 * @param pitch_shifter The pitch shifter.
 * 
 * @return void
*/
void free_gs_pitchshift(GS_pitchshift* pitch_shifter);


#endif