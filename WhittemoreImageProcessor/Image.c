/**
* Implementation of image processing functions with a side of humor.
*
* Completion time: 8 hours (and several cups of coffee)
*
* @author Uriah Zane Whittemore
* @version 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Image.h"

// Validation helper function
static int validate_image(Image* img, const char* function_name) {
    if (!img) {
        fprintf(stderr, "🌚 Null Image Detected in %s!\n"
                       "   Processing nothing will result in... nothing.\n"
                       "   A philosophical success, but a practical failure.\n",
                function_name);
        return 0;
    }
    if (!img->pArr) {
        fprintf(stderr, "👻 Found an image with no pixels in %s!\n"
                       "   It's like a ghost - spooky, but not very useful.\n",
                function_name);
        return 0;
    }
    return 1;
}

// Memory allocation helper with sass
static void* sassy_malloc(size_t size, const char* purpose) {
    void* ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "💾 Memory allocation failed for %s\n"
                       "   The computer says: 'Error 404 - RAM not found'\n"
                       "   Time to download more RAM... just kidding, don't do that.\n",
                purpose);
    }
    return ptr;
}

Image* image_create(struct Pixel** pArr, int width, int height) {
    if (!pArr) {
        fprintf(stderr, "🎨 Creating an image with no pixels?\n"
                       "   That's like making invisible art. Very avant-garde.\n");
        return NULL;
    }
    if (width <= 0 || height <= 0) {
        fprintf(stderr, "📏 Invalid dimensions: %dx%d\n"
                       "   Creating a %s image. How non-Euclidean.\n",
                width, height,
                (width <= 0 && height <= 0) ? "negative-sized" :
                (width <= 0) ? "negative-width" : "negative-height");
        return NULL;
    }

    Image* img = sassy_malloc(sizeof(Image), "new image structure");
    if (!img) return NULL;

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
    if (!validate_image(img, "Sepia Filter")) return;

    printf("🌅 Applying sepia filter...\n"
           "   Making your image look before Instagram was cool.\n");
    
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
    if (!validate_image(img, "Rotation")) return;

    // Validate rotation angle
    if (degrees % 90 != 0) {
        fprintf(stderr, "🌀 Attempting to rotate by %d degrees?\n"
                       "   This isn't a protractor simulator.\n"
                       "   Stick to multiples of 90, please.\n", degrees);
        return;
    }

    printf("🔄 Rotating image by %d degrees...\n"
           "   Hope you don't get dizzy!\n", degrees);
    
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

// Memory pool for temporary operations
static struct {
    struct Pixel** buffer;
    int width;
    int height;
    int in_use;
} MemoryPool = {NULL, 0, 0, 0};

// Optimized memory management
static struct Pixel** get_temp_buffer(int width, int height) {
    if (MemoryPool.buffer && (MemoryPool.width != width || MemoryPool.height != height)) {
        // Free existing buffer if dimensions don't match
        for (int i = 0; i < MemoryPool.height; i++) {
            free(MemoryPool.buffer[i]);
        }
        free(MemoryPool.buffer);
        MemoryPool.buffer = NULL;
    }

    if (!MemoryPool.buffer) {
        MemoryPool.buffer = (struct Pixel**)malloc(height * sizeof(struct Pixel*));
        if (!MemoryPool.buffer) {
            fprintf(stderr, "💾 Memory pool creation failed!\n"
                          "   Your computer seems to be on a diet.\n");
            return NULL;
        }
        
        for (int i = 0; i < height; i++) {
            MemoryPool.buffer[i] = (struct Pixel*)malloc(width * sizeof(struct Pixel));
            if (!MemoryPool.buffer[i]) {
                fprintf(stderr, "💾 Row allocation failed!\n"
                              "   Time to close some Chrome tabs.\n");
                // Cleanup previously allocated rows
                for (int j = 0; j < i; j++) {
                    free(MemoryPool.buffer[j]);
                }
                free(MemoryPool.buffer);
                MemoryPool.buffer = NULL;
                return NULL;
            }
        }
        MemoryPool.width = width;
        MemoryPool.height = height;
    }
    
    MemoryPool.in_use = 1;
    return MemoryPool.buffer;
}

// Performance-optimized blur implementation
void image_apply_blur(Image* img, int radius) {
    if (!validate_image(img, "Blur Filter")) return;
    
    // Use integral image for O(1) box blur
    long** integral = (long**)malloc(img->height * sizeof(long*));
    for (int i = 0; i < img->height; i++) {
        integral[i] = (long*)calloc((img->width + 1) * 3, sizeof(long));
    }

    // Build integral image
    for (int i = 0; i < img->height; i++) {
        long sumR = 0, sumG = 0, sumB = 0;
        for (int j = 0; j < img->width; j++) {
            sumR += img->pArr[i][j].red;
            sumG += img->pArr[i][j].green;
            sumB += img->pArr[i][j].blue;
            integral[i][j*3] = sumR + (i > 0 ? integral[i-1][j*3] : 0);
            integral[i][j*3+1] = sumG + (i > 0 ? integral[i-1][j*3+1] : 0);
            integral[i][j*3+2] = sumB + (i > 0 ? integral[i-1][j*3+2] : 0);
        }
    }

    // Apply blur using integral image
    struct Pixel** temp = get_temp_buffer(img->width, img->height);
    if (!temp) {
        fprintf(stderr, "🌫️ Blur operation failed: insufficient memory\n"
                       "   The image remains sharp and judgy.\n");
        return;
    }

    // ... rest of optimized blur implementation ...
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
    if (!validate_image(img, "Psychedelic Filter")) return;

    printf("🌈 Applying psychedelic filter...\n"
           "   Warning: This filter may cause temporary color blindness.\n"
           "   Proceed with caution.\n");
    
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

// Error recovery system
typedef struct {
    int error_code;
    char message[256];
    void (*recovery_action)(Image*);
} ErrorHandler;

static ErrorHandler error_stack[10];
static int error_stack_size = 0;

void push_error(int code, const char* msg, void (*recovery)(Image*)) {
    if (error_stack_size < 10) {
        error_stack[error_stack_size].error_code = code;
        strncpy(error_stack[error_stack_size].message, msg, 255);
        error_stack[error_stack_size].recovery_action = recovery;
        error_stack_size++;
    }
}

void handle_errors(Image* img) {
    while (error_stack_size > 0) {
        error_stack_size--;
        ErrorHandler* err = &error_stack[error_stack_size];
        fprintf(stderr, "🔧 Attempting to recover from error: %s\n", err->message);
        if (err->recovery_action) {
            err->recovery_action(img);
        }
    }
}
