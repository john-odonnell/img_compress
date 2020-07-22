# Image Compression Suite
## Compression and Decompression algorithms based on JPEG compression
**Contributors :: John O'Donnell & Marcus Comolli**

All aspects of compression and decompression are implemeted to meet assignment specifications.\
Compression and Decompression results in 2% loss of image integrity.

**Dependencies**

PNM Library described [here](https://www.cs.tufts.edu/comp/40-2011f/progs/pnmrdr.html).\
David Hanson's [CII Library](https://github.com/drh/cii).

**Architecture**

*image.c*\
Handles command line input, calls compress.c function for (de)compression.

*compress.c*\
compress(): Calls all functions for compression.\
decompress(): Calls all functions for decompression.

*trim_image.c*\
remove_edges(): removes the last column/row so the image has an even number of each.

*float_convert.c*\
to_float(): turns every rgb pixel struct into floating point numbers, constructs a Pnm_rgb_float struct and insert it into a newly allocated array.<br/> 
from_float(): returns the floating point rgb pixel representation and to their original integer struct and inserts them into a newly allocated array. 

*colorspace_trans.c*\
to_colorspace(): converts {r,g,b} to {Y,Pb,Pr}, adds them to a Pnm_colorspace struct.\
from_colorspace(): converts {Y,Pb,Pr} to {r,g,b} in a Pnm_rgb_float struct.

*to_averages.c*\
maintain_range(): ensure b,c,d are in range (-0.3,0.3)\
get_averages(): iterates through 2x2 blocks of pixels and calculates the discrete cosine transform on the pixel brightnesses, along with the average Pb and Pr. These values are stored in a 2d array a quarter the size of the original data structure.\
decompose_averages(): iterates on each block_components struct in an array and uses the inverse discrete cosine transform to reconstruct the brightness of the four pixels represented by the struct's values.

*pack.c*\
pack(): uses Bitpack_newu and Bitpack_news to construct 32 bit codewords out of data members of each block_components struct in a given array.\
unpack(): uses Bitpack_getu and Bitpack_gets to deconstruct a, b, c, d, avgPb and avgPr from a given 32 bit codeword.

*output_compressed.c*\
Uses a loop, Bitpack_get, and putchar to write the codewords to stdout.\

*input_compressed.c*\
Uses a loop, getc, and Bitpack_new to retrieve and store 32 bit codewords.\


**Time Record**

Time spent analyzing: 6 hours
Time spent implementing: 25 hours  
