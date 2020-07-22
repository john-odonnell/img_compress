#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "mem.h"
#include "assert.h"

#include "trim_image.h"
#include "float_convert.h"
#include "colorspace_trans.h"
#include "to_averages.h"
#include "pack.h"
#include "output_compressed.h"
#include "input_compressed.h"

void compress(FILE *input) {
	// establish methods
	A2Methods_T methods = array2_methods_plain;

	// initialize ppm and read from input
	Pnm_ppm *image;
	NEW(image);
	*image = Pnm_ppmread(input, methods);

	// pull height, width and denominator from new ppm
	unsigned denom, width, height;
	height = (*image)->height;
	width  = (*image)->width;
	denom  = (*image)->denominator;
	(void)height; (void)width;

	// remove the right and bottom edge of the image if necesarry
	remove_edges(image);

	// pull the new width and height from the ppm
	unsigned new_height, new_width;
	new_height = (*image)->height;
	new_width  = (*image)->width;

	// convert rgb to floats
	A2Methods_Array2 *floats;
	floats = to_float(methods, image, new_width, new_height, denom);
	// free ppm and pointer
	Pnm_ppmfree(image);
	FREE(image);

	// convert to component video
	A2Methods_Array2 *colorspace;
	colorspace = to_colorspace(methods, floats, new_width, new_height);
	// free floats array and pointer
	methods->free(floats);
	FREE(floats);

	// compute averages
	A2Methods_Array2 *averages;
	averages = get_averages(methods, colorspace, new_width, new_height);
	// free altered and pointer
	methods->free(colorspace);
	FREE(colorspace);

	// pack averages into words
	A2Methods_Array2 *words;
	words = pack(methods, averages);
	// free averages and pointer
	methods->free(averages);
	FREE(averages);
	
	// output words to stdout
	output(words, new_width, new_height);
	
	// free words array and pointer
	methods->free(words);
	FREE(words);

	return;
}

void decompress(FILE *input) {
	// set methods
	A2Methods_T methods = array2_methods_plain;

	// initialize decompressed width and height
	unsigned width, height;
	width = 0;
	height = 0;

	// read header from compressed image file
	int read = fscanf(input, "Compressed image format 2\n%u %u", &width, &height);
	assert(read == 2);
	int c = getc(input);
	assert(c == '\n');

	// initialize 2d array of 32 bit words and fill
	A2Methods_Array2 *words;
	words = input_collection(methods, input, width, height);
	
	// convert words to block components
	A2Methods_Array2 *averages;
	averages = unpack(methods, words);
	// free words array and pointer
	methods->free(words);
	FREE(words);
	
	// convert block components to pixels of colorspace
	A2Methods_Array2 *colorspace;
	colorspace = decompose_averages(methods, averages, width, height);
	// free block componenets array and pointer
	methods->free(averages);
	FREE(averages);

	// convert colorspace piexls to float stucts
	A2Methods_Array2 *floats;
	floats = from_colorspace(methods, colorspace, width, height);
	// free colorspace array and pointer
	methods->free(colorspace);
	FREE(colorspace);

	// initialize new array of int rgb pixels
	// initialize Pnm_ppm of desired members
	A2Methods_Array2 rgb = methods->new(width, height, sizeof(struct Pnm_rgb));
	struct Pnm_ppm pixmap = { .width = width, .height = height, .denominator = 255, .pixels = rgb, .methods = methods };


	// convert float rgb to int rgb
	from_float(methods, floats, &(pixmap.pixels), width, height, 255);
	// free floats array and pointer
	methods->free(floats);
	FREE(floats);
	
	// output to stdout in ppm format
	Pnm_ppmwrite(stdout, &pixmap);
	
	// frees final rgb array memory
	methods->free(&rgb);

	return;
}
