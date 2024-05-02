#ifndef IIR_H
#define IIR_H

#include "utils.h"

//-----------------------------------------------------------------
//structures delcarations
//-----------------------------------------------------------------

/**
 * @brief represent the coefficients of the transfer function
 * 
 * @param index The index of the non-zeros coefficient.
 * 
 * @param val The value of the non-zeros coefficient.
 * 
 * @param N The number of non-zeros coefficients.
 * 
 * @param order The order of the filter.
*/
typedef struct {
    int* index; 
    double* val;
    int N;
    int order;
} Coefficients;

/**
 * @brief represent an IIR filter by it's transfer function coefficients
 * 
 *  @param a The denominator coefficients of the transfer function.
 * 
 * @param b The numerator coefficients of the transfer function.
 * 
 * @param xbuf The input buffer.
 * 
 * @param ybuf The output buffer.
*/
typedef struct {
    Coefficients *a; 
    Coefficients *b;
    data_t* xbuf;
    data_t* ybuf;  
} IIR;


//-----------------------------------------------------------------
//function declarations
//-----------------------------------------------------------------

/**
 * @brief Create an IIR filter
 * 
 * @param a The denominator coefficients of the transfer function.
 * 
 * @param b The numerator coefficients of the transfer function.
 *  
 * @return The dynamic allocated IIR filter
*/
IIR *init_IIR(Coefficients *a, Coefficients *b);

/**
 * @brief Reset the IIR filter internal buffers
 * 
 * @param iir The IIR filter.
*/
void reset_IIR(IIR* iir);

/**
 * @brief Filter a signal with an IIR filter
 * 
 * @param iir The IIR filter.
 * 
 * @param x The input signal.
 * 
 * @param y The output signal.
 * 
 * @param buffer_size The size of the input signal.
 * 
 * @return void
*/
void filter_IIR(IIR* iir, data_t* x, data_t* y, int buffer_size);

/**
 * @brief Free the memory allocated for an IIR filter
 * 
 * @param iir The IIR filter.
 * 
 * @return void
*/
void free_IIR(IIR* iir);

#endif