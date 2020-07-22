#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"

#ifndef TO_AVERAGES_INCLUDED
#define TO_AVERAGES_INCLUDED

typedef struct block_components {
	unsigned  a;
	signed    b,  c,  d;
	unsigned avgPb, avgPr;
} *block_components;

extern A2Methods_Array2 *get_averages(A2Methods_T methods, A2Methods_Array2 *arr, unsigned width, unsigned height);
extern A2Methods_Array2 *decompose_averages(A2Methods_T methods, A2Methods_Array2 *averages, unsigned width, unsigned height);

#endif
