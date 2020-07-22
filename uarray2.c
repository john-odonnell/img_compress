#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "assert.h"
#include "array.h"
#include "arrayrep.h"
#include "mem.h"
#include "uarray2.h"

#define T UArray2_T
struct T {
	int height;
	int width;
	int size;
	Array_T array;

};

// allocate mem for new UArray2_T and the Array_T referenced within
T UArray2_new(int width, int height, int size){
	assert(height>0 && width >0 && size>0);

	T uarray2;
	NEW(uarray2);

	uarray2->height = height;
	uarray2->width = width;
	uarray2->size = size;

	// Array_T *arr;
	// NEW(arr);
	// *arr = Array_new((height*width), size);
	
	uarray2->array = Array_new((height*width), size);

	// uarray2->array = arr;
	return uarray2;
}

// frees memory allocated for Array_T pointed to by UArray2_T, frees the Array_T pointer, and frees UArray2_T
void UArray2_free(T *uarray2){
	assert(uarray2 && *uarray2);
	// FREE((*uarray2)->array);
	// Array_free((*uarray2)->array);
	//FREE((*uarray2)->array);
	Array_free(&((*uarray2)->array));
	FREE(*uarray2);
}

// return 2d array width
int UArray2_width(T uarray2){
	assert(uarray2);
	return uarray2->width;
}

// return 2d array height
int UArray2_height(T uarray2){
	assert(uarray2);
	return uarray2->height;
}

// return size of storage members
int UArray2_size(T uarray2){
	assert(uarray2);
	return uarray2->size;
}

// returns the mem location of the index within the Array_T array that corresponds to the (i, j) location
// within the 2d array. This is through the relationship idx = (i * width) + j
// (i * width) corresponds to the beginning of the row in question, +j adds specificity
//
// apparently not i is column and j is row
void *UArray2_at(T uarray2, int i, int j){
	assert(uarray2);
	assert(j>=0 && j<uarray2->height);
	assert(i>=0 && i<uarray2->width);
	
	// return ((uarray2->array + (height * uarray2->width * uarray2->size)) + (width * uarray2->size));
	return Array_get(uarray2->array, ((j * uarray2->width) + i));
}

// maps the apply() function to all members of the array in row major order
void UArray2_map_row_major(T uarray2, void apply(int i, int j, T uarray2, void *elem, void *cl), void *cl){
 	void *n;
	int height = uarray2->height;
	int width = uarray2->width;
	int size = uarray2->size;
	Array_T *arr = &(uarray2->array);

	for (int j=0; j<height; j++) {
		for (int i=0; i<width; i++) {
 			n = (void *)((*arr)->array + (j * size * width) + (i * size));
 			apply(i, j, uarray2, n, cl);
		}
	}
	return;
}

// maps the apply() function to all members of the array in column major order
void UArray2_map_col_major(T uarray2, void apply(int i, int j, T uarray2, void *elem, void *cl), void *cl){
 	void *n;
	Array_T *arr = &(uarray2->array);
	for(int i=0; i<uarray2->width; i++){
 		for(int j=0; j<uarray2->height; j++){
 			n = (void *)((*arr)->array + (j * uarray2->size * uarray2->width) + (i * uarray2->size));
 			apply(i, j, uarray2, n, cl);
 		}
 	}
}

#undef T
