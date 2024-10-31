/**
* Implementation of several functions to manipulate BMP files.
*
* Completion time: 6 hours
*
* @author Your Name Here
* @version 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BMPHandler.h"

#define BMP_HEADER_SIZE 14
#define DIB_HEADER_SIZE 40
#define BITS_PER_PIXEL 24

int readBMPHeader(FILE* file, struct BMP_Header* header) {
    // Validate input parameters
    if (!file || !header) {
        fprintf(stderr, "🤦 Attempting to read a header from nothing?\n"
                       "   The void stares back, but it doesn't contain BMP headers.\n");
        return -1;
    }

    // Read header data
    size_t read_count = 0;
    read_count += fread(header->signature, sizeof(char), 2, file);
    read_count += fread(&header->size, sizeof(int), 1, file);
    read_count += fread(&header->reserved1, sizeof(short), 1, file);
    read_count += fread(&header->reserved2, sizeof(short), 1, file);
    read_count += fread(&header->offset_pixel_array, sizeof(int), 1, file);

    // Verify complete header read
    if (read_count != 6) {
        fprintf(stderr, "📑 Found only part of a BMP header.\n"
                       "   Did the rest of it go on vacation?\n");
        return -1;
    }

    // Validate BMP signature
    if (header->signature[0] != 'B' || header->signature[1] != 'M') {
        fprintf(stderr, "🎭 This file is pretending to be a BMP.\n"
                       "   Found '%c%c' instead of 'BM'. Identity theft is not a joke!\n",
                       header->signature[0], header->signature[1]);
        return -1;
    }

    return 0;
}

void writeBMPHeader(FILE* file, struct BMP_Header* header) {
    fwrite(header->signature, sizeof(char), 2, file);
    fwrite(&header->size, sizeof(int), 1, file);
    fwrite(&header->reserved1, sizeof(short), 1, file);
    fwrite(&header->reserved2, sizeof(short), 1, file);
    fwrite(&header->offset_pixel_array, sizeof(int), 1, file);
}

void readDIBHeader(FILE* file, struct DIB_Header* header) {
    fread(&header->size, sizeof(int), 1, file);
    fread(&header->width, sizeof(int), 1, file);
    fread(&header->height, sizeof(int), 1, file);
    fread(&header->planes, sizeof(short), 1, file);
    fread(&header->bits_per_pixel, sizeof(short), 1, file);
    fread(&header->compression, sizeof(int), 1, file);
    fread(&header->image_size, sizeof(int), 1, file);
    fread(&header->x_resolution, sizeof(int), 1, file);
    fread(&header->y_resolution, sizeof(int), 1, file);
    fread(&header->colors, sizeof(int), 1, file);
    fread(&header->important_colors, sizeof(int), 1, file);
}

void writeDIBHeader(FILE* file, struct DIB_Header* header) {
    fwrite(&header->size, sizeof(int), 1, file);
    fwrite(&header->width, sizeof(int), 1, file);
    fwrite(&header->height, sizeof(int), 1, file);
    fwrite(&header->planes, sizeof(short), 1, file);
    fwrite(&header->bits_per_pixel, sizeof(short), 1, file);
    fwrite(&header->compression, sizeof(int), 1, file);
    fwrite(&header->image_size, sizeof(int), 1, file);
    fwrite(&header->x_resolution, sizeof(int), 1, file);
    fwrite(&header->y_resolution, sizeof(int), 1, file);
    fwrite(&header->colors, sizeof(int), 1, file);
    fwrite(&header->important_colors, sizeof(int), 1, file);
}

void makeBMPHeader(struct BMP_Header* header, int width, int height) {
    header->signature[0] = 'B';
    header->signature[1] = 'M';
    header->size = 54 + (width * height * 3); // 54 bytes for headers + image data
    header->reserved1 = 0;
    header->reserved2 = 0;
    header->offset_pixel_array = 54; // 14 + 40 bytes (BMP + DIB headers)
}

void makeDIBHeader(struct DIB_Header* header, int width, int height) {
    header->size = 40;
    header->width = width;
    header->height = height;
    header->planes = 1;
    header->bits_per_pixel = 24;
    header->compression = 0;
    header->image_size = width * height * 3;
    header->x_resolution = 3780;
    header->y_resolution = 3780;
    header->colors = 0;
    header->important_colors = 0;
}

void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height) {
    // Calculate padding
    int padding = (4 - (width * 3) % 4) % 4;
    
    // Read pixels from bottom to top (BMP format)
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            fread(&pArr[i][j].blue, sizeof(unsigned char), 1, file);
            fread(&pArr[i][j].green, sizeof(unsigned char), 1, file);
            fread(&pArr[i][j].red, sizeof(unsigned char), 1, file);
        }
        // Skip padding bytes
        fseek(file, padding, SEEK_CUR);
    }
}

void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height) {
    // Calculate padding
    int padding = (4 - (width * 3) % 4) % 4;
    unsigned char pad[3] = {0, 0, 0};
    
    // Write pixels from bottom to top
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            fwrite(&pArr[i][j].blue, sizeof(unsigned char), 1, file);
            fwrite(&pArr[i][j].green, sizeof(unsigned char), 1, file);
            fwrite(&pArr[i][j].red, sizeof(unsigned char), 1, file);
        }
        // Write padding bytes
        fwrite(pad, sizeof(unsigned char), padding, file);
    }
}