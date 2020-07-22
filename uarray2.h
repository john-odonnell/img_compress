#include "array.h"

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED
#define T UArray2_T

typedef struct T *T;

extern T UArray2_new (int width, int height, int size);
// returns a new UArray2_T struct allocated on the heap based on
// the given height, width and size
extern void UArray2_free (T *uarray2);
// frees the memory allocated for the UArray2_T struct and the contained array

extern int UArray2_width (T uarray2);
// returns the width of a given UArray2_T
extern int UArray2_height (T uarray2);
// returns the height of a given UArray2_T
extern int UArray2_size (T uarray2);
// returns the size of each entry in a given UArray2_T

extern void *UArray2_at (T uarray2, int i, int j);
// returns a void pointer to the memory location within a given UArray2_T
// that represent by column i, row j

extern void UArray2_map_row_major (T uarray2, void apply(int i, int j, T uarray2, void *elem, void *cl), void *cl);
// maps the apply() function to each idx of a given UArray2_T in row major order
extern void UArray2_map_col_major (T uarray2, void apply(int i, int j, T uarray2, void *elem, void *cl), void *cl);
// maps the apply() function to each idx of a given UArray2_T in column major order

#undef T
#endif
