#include "pnm.h"

void remove_edges(Pnm_ppm *ppm) {
	// pull width and height from ppm
	int width, height;
	width  = (*ppm)->width;
	height = (*ppm)->height;
	
	// initialize new width and height
	int new_height, new_width;

	if (height%2!=0)		{ new_height = height - 1; }
			else		{ new_height = height; }
	if (width%2 !=0) 		{ new_width = width - 1; }
			else		{ new_width = width; }
	
	// change width and height of ppm
	(*ppm)->width = new_width;
	(*ppm)->height = new_height;

	return;
}
