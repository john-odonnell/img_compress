#include <stdio.h>
#include <stdint.h>

#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "bitpack.h"

void output(A2Methods_Array2 *arr, unsigned width, unsigned height) {
	A2Methods_T methods = array2_methods_plain;

	int comp_width = methods->width(*arr);
	int comp_height = methods->height(*arr);
	
	printf("Compressed image format 2\n%u %u", width, height);
	printf("\n");
	for (int j = 0; j < comp_height; j++) {
		for (int i = 0; i < comp_width; i++) {
			uint64_t codeword = *(uint64_t *)methods->at(*arr, i, j);
			for (int k = 3; k >= 0; k--) {
				char byte = (char)Bitpack_getu(codeword, 8, (k * 8));
				putchar(byte);
			}
		}
	}
	printf("\n");

	return;
}
