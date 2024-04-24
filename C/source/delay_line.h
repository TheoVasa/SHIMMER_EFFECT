#ifndef DELAY_LINE_H
#define DELAY_LINE_H
//-----------------------------------------------------------------
//macros declarations
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//structures delcarations
//-----------------------------------------------------------------

/**
 * @brief represent a delay line
 * 
 * @param delay The delay of the delay line.
 * 
 * @param buffer The internal buffer of the delay line.
*/
typedef struct {
    int delay; 
    double* buffer;
} DelayLine;

//-----------------------------------------------------------------
//function declarations
//-----------------------------------------------------------------

/**
 * @brief Create a delay line
 * 
 * @param delay The delay of the delay line.
 * 
 * @return The dynamic allocated delay line
*/
DelayLine* init_delay_line(int delay);

/**
 * @brief Reset the delay line internal buffers
 * 
 * @param delay_line The delay line.
*/
void reset_delay_line(DelayLine* delay_line);

/**
 * @brief Apply the delay line to a signal
 * 
 * @param delay_line The delay line.
 * 
 * @param x The input buffer.
 * 
 * @param y The output buffer.
 * 
 * @param buffer_size The size of the buffer.
*/
void apply_delay_line(DelayLine* delay_line, double* x, double* y, int buffer_size);

/**
 * @brief Free the memory allocated by the delay line
 * 
 * @param delay_line The delay line.
 * 
 * @return void
*/
void free_delay_line(DelayLine* delay_line);

#endif