#include <stdio.h>

#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "mem.h"
#include "assert.h"

#include "float_convert.h"
#include "colorspace_trans.h"

A2Methods_Array2 * to_colorspace(A2Methods_T methods, A2Methods_Array2 *arr, unsigned width, unsigned height) {
	// initialize float variables
	float red, green, blue;
	float   Y,    Pb,   Pr;

	A2Methods_Array2 *colorspace;
	NEW(colorspace);
	*colorspace = methods->new(width, height, sizeof(struct Pnm_colorspace));
	
	// establish void pointer for inter-type assignment
	Pnm_rgb_float this_pix_f;
	Pnm_colorspace destination;

	// for each rgb float set in the array
	for (int j = 0; j < (int)height; j++) {
		for (int i = 0; i < (int)width; i++) {
			// pull memory location of pixel
			this_pix_f = methods->at(*arr, i, j);

			// pull rgb
			red   = this_pix_f->red;
			green = this_pix_f->green;
			blue  = this_pix_f->blue;

			// set Y/Pb/Pr
			Y  =     (0.299 * red) +    (0.587 * green) +    (0.114 * blue);
			Pb = (-0.168736 * red) - (0.331264 * green) +      (0.5 * blue);
			Pr =       (0.5 * red) - (0.418688 * green) - (0.081312 * blue);
			
			// assert Pb and Pr within range
			assert(Y  >=  0.0 &&  Y <= 1.0); 
			assert(Pb >= -0.5 && Pb <= 0.5);
			assert(Pr >= -0.5 && Pr <= 0.5);

			// set destination
			destination = methods->at(*colorspace, i, j);
			destination->Y  = Y;
			destination->Pb = Pb;
			destination->Pr = Pr;
		}
	}

	return colorspace;
}

A2Methods_Array2 *from_colorspace(A2Methods_T methods, A2Methods_Array2 *colorspace, unsigned width, unsigned height) {
	// initialize local float variables
	float red, green, blue;
	float   Y,    Pb,   Pr;
	
	// initialize pointers
	Pnm_colorspace this_pix;
	Pnm_rgb_float destination;
	
	// initialize new 2d array for float structs
	A2Methods_Array2 *floats;
	NEW(floats);
	*floats = methods->new((int)width, (int)height, sizeof(struct Pnm_rgb_float));
	
	// for each pixel
	for (int j = 0; j < (int)height; j++) {
		for (int i = 0; i < (int)width; i++) {
			// pull colorspace location
			this_pix = methods->at(*colorspace, i, j);
			
			// pull colorspace data members
			Y  = this_pix->Y;
			Pb = this_pix->Pb;
			Pr = this_pix->Pr;
			
			// calculate float rgb from colorspace
			red   = (1.0 * Y) +      (0.0 * Pb) +    (1.402 * Pr);
			green = (1.0 * Y) - (0.344136 * Pb) - (0.714136 * Pr);
			blue  = (1.0 * Y) +    (1.772 * Pb) +      (0.0 * Pr);
			
			// pull destination location
			// and assign members
			destination = methods->at(*floats, i, j);
			destination->red   = red;
			destination->green = green;
			destination->blue  = blue;
		}
	}

	// return pointer
	return floats;
}
