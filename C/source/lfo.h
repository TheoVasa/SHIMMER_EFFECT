#ifndef LFO_H
#define LFO_H

#include "utils.h"
//-----------------------------------------------------------------
//macros declarations
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//structures delcarations
//-----------------------------------------------------------------

/**
 * @brief represent a Low Frequency Oscillator
 * 
 * @param amp The amplitude of the LFO.
 * 
 * @param freq The frequency of the LFO.
 * 
 * @param start_time The start time of the LFO.
 * 
 * @param running The running state of the LFO.
*/
typedef struct  {
    double *amp;
    double *freq;
    float *start_time; 
    int *running;
} LFO;


//-----------------------------------------------------------------
//function declarations
//-----------------------------------------------------------------

/**
 * @brief Create a Low Frequency Oscillator
 * 
 * @param freq The frequency of the LFO.
 * 
 * @param amp The amplitude of the LFO.
 * 
*/
LFO *init_lfo(double freq, double amp);


/**
 * @brief Start or reset the LFO
 * 
 * @param lfo The LFO.
 * 
 * @return void
*/
void start_lfo(LFO* lfo);

/**
 * @brief Stop the LFO
 * 
 * @param lfo The LFO.
 * 
 * @return void
*/
void stop_lfo(LFO* lfo);

/**
 * @brief Get the value of the LFO
 * 
 * @param lfo The LFO.
 * 
 * @return The value of the LFO
*/
data_t lfo_get_value(LFO* lfo);

/**
 * @brief Free the memory allocated by the LFO
 * 
 * @param lfo The LFO.
 * 
 * @return void
*/
void free_lfo(LFO* lfo);

#endif