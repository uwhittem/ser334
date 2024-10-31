#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Image.h"

// Required functions
Image* image_create(struct Pixel** pArr, int width, int height) {
    Image* img = (Image*)malloc(sizeof(Image));
    img->pArr = pArr;
    img->width = width;
    img->height = height;
    return img;
}

void image_destroy(Image** img) {
    if (img != NULL && *img != NULL) {
        // Note: we don't free pArr as per requirements
        free(*img);
        *img = NULL;
    }
}

struct Pixel** image_get_pixels(Image* img) {
    return img->pArr;
}

int image_get_width(Image* img) {
    return img->width;
}

int image_get_height(Image* img) {
    return img->height;
}

// Required filters
void image_apply_bw(Image* img) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            unsigned char gray = (unsigned char)(
                0.299 * img->pArr[i][j].red +
                0.587 * img->pArr[i][j].green +
                0.114 * img->pArr[i][j].blue
            );
            img->pArr[i][j].red = gray;
            img->pArr[i][j].green = gray;
            img->pArr[i][j].blue = gray;
        }
    }
}

void image_apply_colorshift(Image* img, int rShift, int gShift, int bShift) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            // Apply shifts and clamp values between 0 and 255
            img->pArr[i][j].red = (unsigned char)fmax(0, fmin(255, img->pArr[i][j].red + rShift));
            img->pArr[i][j].green = (unsigned char)fmax(0, fmin(255, img->pArr[i][j].green + gShift));
            img->pArr[i][j].blue = (unsigned char)fmax(0, fmin(255, img->pArr[i][j].blue + bShift));
        }
    }
}

// Required resize implementation
void image_apply_resize(Image* img, float factor) {
    int new_width = (int)(img->width * factor);
    int new_height = (int)(img->height * factor);
    
    // Allocate new pixel array
    struct Pixel** new_pArr = (struct Pixel**)malloc(new_height * sizeof(struct Pixel*));
    for (int i = 0; i < new_height; i++) {
        new_pArr[i] = (struct Pixel*)malloc(new_width * sizeof(struct Pixel));
    }
    
    // Nearest neighbor interpolation
    for (int i = 0; i < new_height; i++) {
        for (int j = 0; j < new_width; j++) {
            int src_i = (int)(i / factor);
            int src_j = (int)(j / factor);
            
            // Clamp source coordinates
            src_i = fmin(src_i, img->height - 1);
            src_j = fmin(src_j, img->width - 1);
            
            new_pArr[i][j] = img->pArr[src_i][src_j];
        }
    }
    
    // Free old pixel array
    for (int i = 0; i < img->height; i++) {
        free(img->pArr[i]);
    }
    free(img->pArr);
    
    // Update image with new array and dimensions
    img->pArr = new_pArr;
    img->width = new_width;
    img->height = new_height;
}

// Sepia filter
void image_apply_sepia(Image* img) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            int tr = (int)(0.393 * img->pArr[i][j].red + 0.769 * img->pArr[i][j].green + 0.189 * img->pArr[i][j].blue);
            int tg = (int)(0.349 * img->pArr[i][j].red + 0.686 * img->pArr[i][j].green + 0.168 * img->pArr[i][j].blue);
            int tb = (int)(0.272 * img->pArr[i][j].red + 0.534 * img->pArr[i][j].green + 0.131 * img->pArr[i][j].blue);
            
            img->pArr[i][j].red = (unsigned char)fmin(255, tr);
            img->pArr[i][j].green = (unsigned char)fmin(255, tg);
            img->pArr[i][j].blue = (unsigned char)fmin(255, tb);
        }
    }
}

// Rotation filter
void image_apply_rotate(Image* img, int degrees) {
    // Only support 90, 180, 270 degrees for simplicity
    int rotations = ((degrees % 360) / 90) % 4;
    if (rotations == 0) return;
    
    struct Pixel** new_pArr;
    int new_width, new_height;
    
    if (rotations % 2 == 1) {
        // 90 or 270 degrees - dimensions swap
        new_width = img->height;
        new_height = img->width;
    } else {
        // 180 degrees - dimensions stay the same
        new_width = img->width;
        new_height = img->height;
    }
    
    // Allocate new array
    new_pArr = (struct Pixel**)malloc(new_height * sizeof(struct Pixel*));
    for (int i = 0; i < new_height; i++) {
        new_pArr[i] = (struct Pixel*)malloc(new_width * sizeof(struct Pixel));
    }
    
    // Copy pixels with rotation
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            int new_i, new_j;
            switch (rotations) {
                case 1: // 90 degrees
                    new_i = j;
                    new_j = img->height - 1 - i;
                    break;
                case 2: // 180 degrees
                    new_i = img->height - 1 - i;
                    new_j = img->width - 1 - j;
                    break;
                case 3: // 270 degrees
                    new_i = img->width - 1 - j;
                    new_j = i;
                    break;
            }
            new_pArr[new_i][new_j] = img->pArr[i][j];
        }
    }
    
    // Free old array and update image
    for (int i = 0; i < img->height; i++) {
        free(img->pArr[i]);
    }
    free(img->pArr);
    
    img->pArr = new_pArr;
    img->width = new_width;
    img->height = new_height;
}

// Flip filters
void image_apply_flip_horizontal(Image* img) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width / 2; j++) {
            struct Pixel temp = img->pArr[i][j];
            img->pArr[i][j] = img->pArr[i][img->width - 1 - j];
            img->pArr[i][img->width - 1 - j] = temp;
        }
    }
}

void image_apply_flip_vertical(Image* img) {
    for (int i = 0; i < img->height / 2; i++) {
        for (int j = 0; j < img->width; j++) {
            struct Pixel temp = img->pArr[i][j];
            img->pArr[i][j] = img->pArr[img->height - 1 - i][j];
            img->pArr[img->height - 1 - i][j] = temp;
        }
    }
}

// Brightness adjustment
void image_apply_brightness(Image* img, int adjustment) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            img->pArr[i][j].red = (unsigned char)fmax(0, fmin(255, img->pArr[i][j].red + adjustment));
            img->pArr[i][j].green = (unsigned char)fmax(0, fmin(255, img->pArr[i][j].green + adjustment));
            img->pArr[i][j].blue = (unsigned char)fmax(0, fmin(255, img->pArr[i][j].blue + adjustment));
        }
    }
}

// Contrast adjustment
void image_apply_contrast(Image* img, float factor) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            float fr = ((img->pArr[i][j].red - 128) * factor) + 128;
            float fg = ((img->pArr[i][j].green - 128) * factor) + 128;
            float fb = ((img->pArr[i][j].blue - 128) * factor) + 128;
            
            img->pArr[i][j].red = (unsigned char)fmax(0, fmin(255, fr));
            img->pArr[i][j].green = (unsigned char)fmax(0, fmin(255, fg));
            img->pArr[i][j].blue = (unsigned char)fmax(0, fmin(255, fb));
        }
    }
}

// Edge detection (simple Sobel operator)
void image_apply_edge_detection(Image* img) {
    // First convert to grayscale
    image_apply_bw(img);
    
    struct Pixel** temp = (struct Pixel**)malloc(img->height * sizeof(struct Pixel*));
    for (int i = 0; i < img->height; i++) {
        temp[i] = (struct Pixel*)malloc(img->width * sizeof(struct Pixel));
        memcpy(temp[i], img->pArr[i], img->width * sizeof(struct Pixel));
    }
    
    for (int i = 1; i < img->height - 1; i++) {
        for (int j = 1; j < img->width - 1; j++) {
            // Simplified Sobel operator
            int gx = 
                temp[i-1][j+1].red + 2*temp[i][j+1].red + temp[i+1][j+1].red -
                temp[i-1][j-1].red - 2*temp[i][j-1].red - temp[i+1][j-1].red;
            
            int gy = 
                temp[i+1][j-1].red + 2*temp[i+1][j].red + temp[i+1][j+1].red -
                temp[i-1][j-1].red - 2*temp[i-1][j].red - temp[i-1][j+1].red;
            
            int magnitude = (int)sqrt(gx*gx + gy*gy);
            magnitude = fmin(255, magnitude);
            
            img->pArr[i][j].red = img->pArr[i][j].green = img->pArr[i][j].blue = magnitude;
        }
    }
    
    // Free temporary array
    for (int i = 0; i < img->height; i++) {
        free(temp[i]);
    }
    free(temp);
}

// Blur effect (box blur)
void image_apply_blur(Image* img, int radius) {
    struct Pixel** temp = (struct Pixel**)malloc(img->height * sizeof(struct Pixel*));
    for (int i = 0; i < img->height; i++) {
        temp[i] = (struct Pixel*)malloc(img->width * sizeof(struct Pixel));
    }
    
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            int r = 0, g = 0, b = 0, count = 0;
            
            for (int di = -radius; di <= radius; di++) {
                for (int dj = -radius; dj <= radius; dj++) {
                    int ni = i + di;
                    int nj = j + dj;
                    
                    if (ni >= 0 && ni < img->height && nj >= 0 && nj < img->width) {
                        r += img->pArr[ni][nj].red;
                        g += img->pArr[ni][nj].green;
                        b += img->pArr[ni][nj].blue;
                        count++;
                    }
                }
            }
            
            temp[i][j].red = r / count;
            temp[i][j].green = g / count;
            temp[i][j].blue = b / count;
        }
    }
    
    // Copy back and free temp array
    for (int i = 0; i < img->height; i++) {
        memcpy(img->pArr[i], temp[i], img->width * sizeof(struct Pixel));
        free(temp[i]);
    }
    free(temp);
}

// Color inversion
void image_apply_invert(Image* img) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            img->pArr[i][j].red = 255 - img->pArr[i][j].red;
            img->pArr[i][j].green = 255 - img->pArr[i][j].green;
            img->pArr[i][j].blue = 255 - img->pArr[i][j].blue;
        }
    }
}

// Psychedelic effect
void image_apply_psychedelic(Image* img) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            unsigned char r = img->pArr[i][j].red;
            unsigned char g = img->pArr[i][j].green;
            unsigned char b = img->pArr[i][j].blue;
            
            img->pArr[i][j].red = (r * 0.5 + g * 0.5);
            img->pArr[i][j].green = (g * 0.5 + b * 0.5);
            img->pArr[i][j].blue = (b * 0.5 + r * 0.5);
        }
    }
}

// Vignette effect
void image_apply_vignette(Image* img, float strength) {
    float centerX = img->width / 2.0f;
    float centerY = img->height / 2.0f;
    float maxDist = sqrt(centerX * centerX + centerY * centerY);
    
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            float dx = j - centerX;
            float dy = i - centerY;
            float distance = sqrt(dx * dx + dy * dy);
            float factor = 1.0f - (distance / maxDist) * strength;
            factor = fmax(0, factor);
            
            img->pArr[i][j].red = (unsigned char)(img->pArr[i][j].red * factor);
            img->pArr[i][j].green = (unsigned char)(img->pArr[i][j].green * factor);
            img->pArr[i][j].blue = (unsigned char)(img->pArr[i][j].blue * factor);
        }
    }
}
