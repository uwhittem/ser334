/**
 * THIS IS NOT A BASE FILE.
 *
 * The following code is a minimal example showing how to apply a blue color shift to the ttt.bmp test file using
 * the functionality you will implement in BMPHandler.h and Image.h. It serves as an example of the syntax that
 * your implementation should support. This code will not function until you have completed all the functionality in the
 * header files.
 *
 * DO NOT SUBMIT THIS FILE WITH YOUR HOMEWORK.
 *
 * Completion time: 334 minutes
 *
 * @author Vatricia Edgar, Ruben Acuna
 * @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include "BMPHandler.h"
#include "Image.h"

void main() {
	//START - HEADER FUNCTIONALITY MINIMAL EXAMPLE (DO NOT SUBMIT)
	struct BMP_Header BMP;
	struct DIB_Header DIB;

	FILE* file_input = fopen("ttt.bmp", "rb");
	readBMPHeader(file_input, &BMP);
	readDIBHeader(file_input, &DIB);

	struct Pixel** pixels = (struct Pixel**)malloc(sizeof(struct Pixel*) * 152);
	for (int p = 0; p < 152; p++) {
		pixels[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * 152);
	}
	readPixelsBMP(file_input, pixels, 152, 152);
	fclose(file_input);

	Image* img = image_create(pixels, 152, 152);
	image_apply_colorshift(img, 0, 0, 40);

	FILE* file_output = fopen("ttt_usagesample.bmp", "wb");
	writeBMPHeader(file_output, &BMP);
	writeDIBHeader(file_output, &DIB);
	writePixelsBMP(file_output, image_get_pixels(img), image_get_width(img), image_get_width(img));
	image_destroy(img);
	fclose(file_output);
	
	//END - HEADER FUNCTIONALITY MINIMAL EXAMPLE (DO NOT SUBMIT)
}