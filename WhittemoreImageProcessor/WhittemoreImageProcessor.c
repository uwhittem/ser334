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
#include <string.h>
#include <unistd.h>
#include "BMPHandler.h"
#include "Image.h"

void print_usage() {
    printf("\n🎨 WhittemoreImageProcessor: Because MS Paint was too mainstream 🎨\n");
    printf("\nUsage: WhittemoreImageProcessor input_file [options]\n");
    printf("Options:\n");
    printf("  -o <file>      Output file name (default: output.bmp)\n");
    printf("  -w             Convert to grayscale\n");
    printf("  -r <value>     Red color shift\n");
    printf("  -g <value>     Green color shift\n");
    printf("  -b <value>     Blue color shift\n");
    printf("  -s <factor>    Scale image by factor\n");
    printf("\nAdditional filters:\n");
    printf("  --sepia        Apply sepia filter\n");
    printf("  --rotate <deg> Rotate image (90, 180, or 270 degrees)\n");
    printf("  --fliph        Flip horizontally\n");
    printf("  --flipv        Flip vertically\n");
    printf("  --bright <val> Adjust brightness (-255 to 255)\n");
    printf("  --contrast <f> Adjust contrast (factor > 0)\n");
    printf("  --edge         Apply edge detection\n");
    printf("  --blur <rad>   Apply blur (radius > 0)\n");
    printf("  --invert       Invert colors\n");
    printf("  --psych        Apply psychedelic effect\n");
    printf("  --vignette <s> Apply vignette effect (0-1)\n");
}

// Custom error handling with personality
void print_error(const char* message, int error_type) {
    switch(error_type) {
        case 1: // File errors
            fprintf(stderr, "🚫 File Crisis! %s\n"
                          "   (Did the file run away? Is it hiding under the bed?)\n", message);
            break;
        case 2: // Parameter errors
            fprintf(stderr, "🤔 Parameter Puzzle: %s\n"
                          "   (The computer is confused, and frankly, so are we)\n", message);
            break;
        case 3: // Memory errors
            fprintf(stderr, "💾 Memory Meltdown: %s\n"
                          "   (Have you tried downloading more RAM?)\n", message);
            break;
        case 4: // Format errors
            fprintf(stderr, "📸 Image Identity Crisis: %s\n"
                          "   (This file thinks it's a BMP, but we have trust issues)\n", message);
            break;
        default:
            fprintf(stderr, "❌ Unexpected Error: %s\n"
                          "   (Even the error handler is confused)\n", message);
    }
}

// Add this function for progress reporting
void report_progress(const char* operation, int progress, int total) {
    const int bar_width = 40;
    float percentage = (float)progress / total;
    int filled = (int)(bar_width * percentage);
    
    printf("\r%s: [", operation);
    for (int i = 0; i < bar_width; i++) {
        if (i < filled) printf("█");
        else printf(" ");
    }
    printf("] %.1f%%", percentage * 100);
    fflush(stdout);
    
    if (progress == total) printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_error("Running an image processor without an image? That's like making a sandwich without bread!", 2);
        print_usage();
        return 1;
    }

    // Default values
    char* input_file = argv[1];
    char* output_file = "output.bmp";
    int rshift = 0, gshift = 0, bshift = 0;
    float scale_factor = 1.0;
    int apply_grayscale = 0;
    
    // Additional filter flags
    int apply_sepia = 0;
    int rotation_degrees = 0;
    int flip_horizontal = 0;
    int flip_vertical = 0;
    int brightness = 0;
    float contrast = 1.0;
    int edge_detect = 0;
    int blur_radius = 0;
    int invert = 0;
    int psychedelic = 0;
    float vignette = 0.0;

    // Parse command line arguments
    int i;
    for (i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(argv[i], "-w") == 0) {
            apply_grayscale = 1;
        } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            rshift = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-g") == 0 && i + 1 < argc) {
            gshift = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
            bshift = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            scale_factor = atof(argv[++i]);
        } else if (strcmp(argv[i], "--sepia") == 0) {
            apply_sepia = 1;
        } else if (strcmp(argv[i], "--rotate") == 0 && i + 1 < argc) {
            rotation_degrees = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--fliph") == 0) {
            flip_horizontal = 1;
        } else if (strcmp(argv[i], "--flipv") == 0) {
            flip_vertical = 1;
        } else if (strcmp(argv[i], "--bright") == 0 && i + 1 < argc) {
            brightness = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--contrast") == 0 && i + 1 < argc) {
            contrast = atof(argv[++i]);
        } else if (strcmp(argv[i], "--edge") == 0) {
            edge_detect = 1;
        } else if (strcmp(argv[i], "--blur") == 0 && i + 1 < argc) {
            blur_radius = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--invert") == 0) {
            invert = 1;
        } else if (strcmp(argv[i], "--psych") == 0) {
            psychedelic = 1;
        } else if (strcmp(argv[i], "--vignette") == 0 && i + 1 < argc) {
            vignette = atof(argv[++i]);
        }
    }

    // Open input file
    FILE* file_input = fopen(input_file, "rb");
    if (!file_input) {
        print_error("Error: Cannot open input file", 1);
        return 1;
    }

    // Read headers
    struct BMP_Header bmp_header;
    struct DIB_Header dib_header;
    readBMPHeader(file_input, &bmp_header);
    readDIBHeader(file_input, &dib_header);

    // Allocate pixel array
    struct Pixel** pixels = (struct Pixel**)malloc(dib_header.height * sizeof(struct Pixel*));
    for (int i = 0; i < dib_header.height; i++) {
        pixels[i] = (struct Pixel*)malloc(dib_header.width * sizeof(struct Pixel));
    }

    // Read pixels
    readPixelsBMP(file_input, pixels, dib_header.width, dib_header.height);
    fclose(file_input);

    // Create image object
    Image* img = image_create(pixels, dib_header.width, dib_header.height);

    // Apply filters in sequence
    printf("\n🎨 Starting image processing adventure...\n");
    int total_operations = apply_grayscale + (rshift || gshift || bshift) + 
                          (scale_factor != 1.0) + apply_sepia + (rotation_degrees != 0) +
                          flip_horizontal + flip_vertical + (brightness != 0) +
                          (contrast != 1.0) + edge_detect + (blur_radius > 0) +
                          invert + psychedelic + (vignette > 0.0);
    int current_operation = 0;

    if (apply_grayscale) {
        image_apply_bw(img);
        report_progress("Processing", ++current_operation, total_operations);
    }
    if (rshift || gshift || bshift) {
        image_apply_colorshift(img, rshift, gshift, bshift);
        report_progress("Processing", ++current_operation, total_operations);
    }
    if (scale_factor != 1.0) {
        image_apply_resize(img, scale_factor);
        report_progress("Processing", ++current_operation, total_operations);
    }
    if (apply_sepia) {
        image_apply_sepia(img);
        report_progress("Processing", ++current_operation, total_operations);
    }
    if (rotation_degrees) {
        image_apply_rotate(img, rotation_degrees);
        report_progress("Processing", ++current_operation, total_operations);
    }
    if (flip_horizontal) {
        image_apply_flip_horizontal(img);
        report_progress("Processing", ++current_operation, total_operations);
    }
    if (flip_vertical) {
        image_apply_flip_vertical(img);
        report_progress("Processing", ++current_operation, total_operations);
    }
    if (brightness) {
        image_apply_brightness(img, brightness);
        report_progress("Processing", ++current_operation, total_operations);
    }
    if (contrast != 1.0) {
        image_apply_contrast(img, contrast);
        report_progress("Processing", ++current_operation, total_operations);
    }
    if (edge_detect) {
        image_apply_edge_detection(img);
        report_progress("Processing", ++current_operation, total_operations);
    }
    if (blur_radius) {
        image_apply_blur(img, blur_radius);
        report_progress("Processing", ++current_operation, total_operations);
    }
    if (invert) {
        image_apply_invert(img);
        report_progress("Processing", ++current_operation, total_operations);
    }
    if (psychedelic) {
        image_apply_psychedelic(img);
        report_progress("Processing", ++current_operation, total_operations);
    }
    if (vignette > 0.0) {
        image_apply_vignette(img, vignette);
        report_progress("Processing", ++current_operation, total_operations);
    }

    // Update headers for potentially resized image
    makeBMPHeader(&bmp_header, img->width, img->height);
    makeDIBHeader(&dib_header, img->width, img->height);

    // Write output file
    FILE* file_output = fopen(output_file, "wb");
    if (!file_output) {
        print_error("Error: Cannot create output file", 1);
        image_destroy(&img);
        return 1;
    }

    writeBMPHeader(file_output, &bmp_header);
    writeDIBHeader(file_output, &dib_header);
    writePixelsBMP(file_output, img->pArr, img->width, img->height);
    fclose(file_output);

    // Cleanup
    image_destroy(&img);
    for (int i = 0; i < dib_header.height; i++) {
        free(pixels[i]);
    }
    free(pixels);

    printf("Image processing complete. Output saved to %s\n", output_file);
    return 0;
}
