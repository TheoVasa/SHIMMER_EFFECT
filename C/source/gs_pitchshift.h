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
 * @param input_size The input number of sample to make GRAIN_SIZE grains
 * 
 * @param jump The jump we have to make to correctly overlap our grains.
 * 
 * @param overlap The number of samples that will be overlaped between two grains.
 * 
 * @param win The window.
 * 
 * @param input_buf The internal input buffer.
 * 
 * @param output_buf The internal output buffer.
 * 
 * @param last_grain The last grain stored.
 * 
*/
typedef struct {
    int shift_factor;
    int grain_size; 
    double fade; 
    int input_size; 
    int jump; 
    int overlap;
    double* win;
    double* input_buf;
    double* output_buf;
    double* last_grain;
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
void filter_gs_pitchshift(GS_pitchshift* pitch_shifter, double* x, double* y, int buffer_size);

/**
 * @brief Free the memory allocated by the pitch shifter
 * 
 * @param pitch_shifter The pitch shifter.
 * 
 * @return void
*/
void free_gs_pitchshift(GS_pitchshift* pitch_shifter);


#endif