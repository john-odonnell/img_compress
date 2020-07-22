#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"

#ifndef COLORSPACE_TRANS_INCLUDED
#define COLORSPACE_TRANS_INCLUDED

typedef struct Pnm_colorspace {
	float Y, Pb, Pr;
} *Pnm_colorspace;

extern A2Methods_Array2 * to_colorspace(A2Methods_T methods, A2Methods_Array2 *arr, unsigned width, unsigned height);
extern A2Methods_Array2 *from_colorspace(A2Methods_T methods, A2Methods_Array2 *colorspace, unsigned width, unsigned height);

#endif
