#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "pnm.h"

#ifndef FLOAT_CONVERT_INCLUDED
#define FLOAT_CONVERT_INCLUDED

typedef struct Pnm_rgb_float {
	float red, green, blue;
} *Pnm_rgb_float;

extern A2Methods_Array2 *to_float(A2Methods_T methods, Pnm_ppm *image, unsigned width, unsigned height, unsigned denom);
extern void from_float(A2Methods_T methods, A2Methods_Array2 *floats, A2Methods_Array2 *rgb, unsigned width, unsigned height, unsigned denom);

#endif
