#ifndef BMPHANDLER_H
#define BMPHANDLER_H

#include <stdio.h>
#include "Image.h"

struct BMP_Header {
    char signature[2];        // ID field ("BM")
    int size;                 // Size of BMP file
    short reserved1;          // Application specific
    short reserved2;          // Application specific
    int offset_pixel_array;   // Offset where pixel array can be found
};

struct DIB_Header {
    int size;                 // Size of DIB header (40 bytes)
    int width;               // Width of image
    int height;              // Height of image
    short planes;            // Number of color planes (must be 1)
    short bits_per_pixel;    // Bits per pixel (24 for our case)
    int compression;         // Compression method (0 for none)
    int image_size;          // Size of raw bitmap data
    int x_resolution;        // Horizontal resolution (3780)
    int y_resolution;        // Vertical resolution (3780)
    int colors;              // Number of colors (0)
    int important_colors;    // Important colors (0)
};

// Function declarations from your existing file
// ... (keep existing function declarations)

#endif
