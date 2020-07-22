#include <stdio.h>
#include <math.h>

#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "mem.h"
#include "to_averages.h"
#include "colorspace_trans.h"
#include "arith411.h"
#include "float_convert.h"

float maintain_range(float x) {
	if (x < -0.3) {
		return -0.3;
	} else if (x > 0.3) {
		return 0.3;
	} else {
		return x;
	}
}

A2Methods_Array2 *get_averages(A2Methods_T methods, A2Methods_Array2 *arr, unsigned width, unsigned height) {
	// initialize array of 2x2 blocks
	A2Methods_Array2 *blocks;
       	NEW(blocks);
	*blocks	= methods->new((int)width/2, (int)height/2, sizeof(struct block_components));
	
	// initialize lum values and
	// Pnm_colorspace pointer
	float    Y1,    Y2, Y3, Y4;
	float     a,     b,  c,  d;
	float totPb, totPr;
	Pnm_colorspace this_pix;

	block_components this_block;

	// for each block
	for (int b_i = 0; b_i < (int)width; b_i+=2) {
		for (int b_j = 0; b_j < (int)height; b_j+=2) {
			
			totPb = 0;
			totPr = 0;
			
			// for each cell in the block
			for (int i = b_i; i < b_i+2; i++) {
				for (int j = b_j; j < b_j+2; j++) {
					// pulll memory location of colorspace struct
					this_pix = methods->at(*arr, i, j);

					// assign Y based on location
					if (i == b_i && j == b_j) {
						Y1 = this_pix->Y;
					} else if (i == b_i+1 && j == b_j) {
						Y2 = this_pix->Y;
					} else if (i == b_i && j == b_j+1) {
						Y3 = this_pix->Y;
					} else {
						Y4 = this_pix->Y;
					}

					// add to total Pb and Pr
					totPb += this_pix->Pb;
					totPr += this_pix->Pr;
				}
			}
			
			// pull destination memory location	
			this_block = methods->at(*blocks, b_i/2, b_j/2);
			// find index of chroma in pre-set table
			this_block->avgPb = Arith_index_of_chroma(totPb/4.0);
			this_block->avgPr = Arith_index_of_chroma(totPr/4.0);
			
			// calculate a, b, c, and d
			a = (Y4 + Y3 + Y2 + Y1)/4.0;
			b = (Y4 + Y3 - Y2 - Y1)/4.0;
			c = (Y4 - Y3 + Y2 - Y1)/4.0;
			d = (Y4 - Y3 - Y2 + Y1)/4.0;
			
			// maintain the range of b, c, and d
			b = maintain_range(b);
			c = maintain_range(c);
			d = maintain_range(d);
			
			// assign a to nine unsigned bits
			// assign the the rest to a -15 to +15 range
			this_block->a = (unsigned)(roundf(a * 511));
			this_block->b =   (signed)(roundf(b *  50));
			this_block->c =   (signed)(roundf(c *  50));
			this_block->d =   (signed)(roundf(d *  50));
		}
	}
	
	// return blocks pointer
	return blocks;
}

A2Methods_Array2 *decompose_averages(A2Methods_T methods, A2Methods_Array2 *averages, unsigned width, unsigned height) {
	// initialize a new 2d array and allocate heap space
	A2Methods_Array2 *floats;
	NEW(floats);
	*floats = methods->new(width, height, sizeof(struct Pnm_rgb_float));
	
	// initialize local variables
	float     a,     b,  c,  d;
	float    Y1,    Y2, Y3, Y4;
	float avgPb, avgPr;
	// initialize pointer
	Pnm_colorspace this_pix;
	block_components this_block;

	// for each block
	for (int b_i = 0; b_i < (int)width/2; b_i++) {
		for (int b_j = 0; b_j < (int)height/2; b_j++) {
			// pull block location
			this_block = methods->at(*averages, b_i, b_j);
	
			// pull block members
			a = (float)(this_block->a)/511.0;
			b = (float)(this_block->b)/ 50.0;
			c = (float)(this_block->c)/ 50.0;
			d = (float)(this_block->d)/ 50.0;
			
			// pull the chroma at the index of Pb and Pr
			avgPb = Arith_chroma_of_index(this_block->avgPb);
			avgPr = Arith_chroma_of_index(this_block->avgPr);
			
			// calculate Y for each pixel in block
			Y1 = a - b - c + d;
			Y2 = a - b + c - d;
			Y3 = a + b - c - d;
			Y4 = a + b + c + d;
			
			// for each pixel in block
			for (int inner_i = 0; inner_i < 2; inner_i++) {
				for (int inner_j = 0; inner_j < 2; inner_j++) {
					// find i and j within new array
					int i = (2 * b_i) + inner_i;
					int j = (2 * b_j) + inner_j;
					
					// pull destination location
					this_pix = methods->at(*floats, i, j);
					
					// assign Pb and Pr
					this_pix->Pb = avgPb;
					this_pix->Pr = avgPr;
					
					// assign Y based on pixel
					if (inner_i == 0 && inner_j == 0) {
						this_pix->Y = Y1;
					} else if (inner_i == 1 && inner_j == 0) {
						this_pix->Y = Y2;
					} else if (inner_i == 0 && inner_j == 1) {
						this_pix->Y = Y3;
					} else {
						this_pix->Y = Y4;
					}
				}
			}
		}
	}
	
	// return pointer
	return floats;
}
