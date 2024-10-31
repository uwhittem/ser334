/**
* Implementation of several functions to manipulate BMP files.
*
* Completion time: ?
*
* @author (your name), Ruben Acuna
* @version 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include "BMPHandler.h"

void readBMPHeader(FILE* file, struct BMP_Header* header) {
    fread(header->signature, sizeof(char), 2, file);
    fread(&header->size, sizeof(int), 1, file);
    fread(&header->reserved1, sizeof(short), 1, file);
    fread(&header->reserved2, sizeof(short), 1, file);
    fread(&header->offset_pixel_array, sizeof(int), 1, file);
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