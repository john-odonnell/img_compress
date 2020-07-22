Pair: Marcus Comolli & John ODonnell

All aspects of compression and decompression are implemeted correctly.
When ppmdiff.c is run given an image and it's decompressed compression, the two are 2% different.

Architecture:
image.c:
	Takes the input from command line and sends the file to the compress.c 
	module based on input. 

compress.c:
	Compress(): Calls all functions for compression.
		Each function in turn transforms data from one 2d array to another,
		and the process finished with an output function.
	Decompress(): Calls all functions for decompression. 

trim_image.c:
	remove_edges(): used to remove last column and/or row if not an 
	even number.

float_convert.c:
	to_float(): turns every rgb pixel struct into floating point numbers,
		constructs a Pnm_rgb_float struct and insert it into a newly allocated array. 
	from_float(): takes the floating point rgb pixel structs and returns
		them to integers, compiles then into a struct, and inserts them
		into a newly allocated array. 

colorspace_trans.c:
	to_colorspace(): converts {r,g,b} to {Y,Pb,Pr}, adds them to a Pnm_colorspace struct and
		inserts them into a newly allocated 2d array
	from_colorspace(): converts {Y,Pb,Pr} to {r,g,b} in a Pnm_rgb_float struct and
		inserts them into a new 2d array

to_averages.c:
	maintain_range(): ensure b,c,d are in range (-0.3,0.3)
	get_averages(): goes through each pixel in each block of 2x2 pixels,
		calculates a, b, c, and d, calculates Y(1-4), and calculates the Pb and Pr
		averages. These data members are added to a block_components stuct and inserted
		into a newly allocated 2d array of half height and hald width of the original.
	decompose_averages(): goes through each block_components struct in an array and, and deconstructs
		Y(1-4) from a, b, c and d. These are added to a Pnm_colorspace struct and inserted into a
		new 2d array.

pack.c: 
	pack(): uses Bitpack_newu and Bitpack_news to construct 32 bit codewords out of data members of
		each block_components struct in a given array. These codewords are added to a new array.
	unpack(): uses Bitpack_getu and Bitpack_gets to deconstruct a, b, c, d, avgPb and avgPr from
		a given 32 bit codeword. The newfound members are added to block_components structs
		and inserted into a new array.

output_compressed.c:
	output(): uses a loop, Bitpack_get, and putchar to write the codewords
	to stdout. 

input_compressed.c:
	input_collection(): uses a loop, getc, and Bitpack_new to retrieve a codeword
	then saves the codewords back to an array.


Time spent analyzing: 6 hours

Time spent solving: 25 hours  
