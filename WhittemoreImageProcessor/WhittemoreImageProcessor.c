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

// Different levels of help menus
void print_simplified_usage() {
    printf("\n🌟 SUPER SIMPLE INSTRUCTIONS (Because reading is hard) 🌟\n\n");
    printf("Step 1️⃣: Find an image file (it ends in .bmp)\n");
    printf("Step 2️⃣: Type this:\n");
    printf("         ./WhittemoreImageProcessor yourimage.bmp\n\n");
    printf("Step 3️⃣: Add ONE of these if you're feeling brave:\n");
    printf("         -w        Make it black & white\n");
    printf("         -r 50     Make it more red\n");
    printf("         -s 2      Make it bigger\n\n");
    printf("Example for the ambitious:\n");
    printf("./WhittemoreImageProcessor cat.bmp -w\n\n");
    printf("(Want the grown-up instructions? Use --help)\n");
}

// Enhanced argument validation
typedef struct {
    const char* option;
    int requires_value;
    const char* sassy_message;
} ValidOption;

static const ValidOption VALID_OPTIONS[] = {
    {"-o", 1, "Output file name needed! Where should the masterpiece be saved?"},
    {"-w", 0, NULL},
    {"-r", 1, "Red shift needs a number! Any number! (Well, -255 to 255)"},
    {"-g", 1, "Green shift requires a value. It's not a mind reader!"},
    {"-b", 1, "Blue shift expects a number. Color theory 101!"},
    {"-s", 1, "Scale factor missing! Size matters!"},
    {"--sepia", 0, NULL},
    {"--rotate", 1, "Rotation needs degrees! (Hint: 90, 180, or 270)"},
    {"--fliph", 0, NULL},
    {"--flipv", 0, NULL},
    {"--bright", 1, "Brightness value needed! (-255 to 255, not 'bright' or 'dark')"},
    {"--contrast", 1, "Contrast factor needed! (Numbers work better than 'more' or 'less')"},
    {"--edge", 0, NULL},
    {"--blur", 1, "Blur radius needed! (A number, not 'kinda blurry')"},
    {"--invert", 0, NULL},
    {"--psych", 0, NULL},
    {"--vignette", 1, "Vignette strength needed! (0-1, not 'Instagram-worthy')"}
};

// Levenshtein distance for "Did you mean?" suggestions
int levenshtein_distance(const char* s1, const char* s2) {
    int len1 = strlen(s1), len2 = strlen(s2);
    int matrix[len1 + 1][len2 + 1];
    
    for (int i = 0; i <= len1; i++) matrix[i][0] = i;
    for (int j = 0; j <= len2; j++) matrix[0][j] = j;
    
    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i-1] == s2[j-1]) ? 0 : 1;
            matrix[i][j] = fmin(matrix[i-1][j] + 1,
                              fmin(matrix[i][j-1] + 1,
                                  matrix[i-1][j-1] + cost));
        }
    }
    return matrix[len1][len2];
}

// Easter egg handling
void handle_easter_egg(const char* arg) {
    if (strcmp(arg, "--coffee") == 0) {
        printf("\n☕ ERROR 418: I'm a teapot\n"
               "    (But here's your virtual coffee anyway)\n"
               "    \n"
               "         )\n"
               "        (\n"
               "      _____)\n"
               "     (_____))\n"
               "    (______)))\n"
               "   (_______)))\n"
               "    |COFFEE|\n"
               "    |     |\n"
               "    |     |\n"
               "    |     |\n"
               "    |     |\n"
               "    |_____|\n\n");
        exit(0);
    } else if (strcmp(arg, "--help-please") == 0) {
        printf("\n🎭 Ah, politeness! How refreshing!\n"
               "Here's your help menu, served with a smile:\n");
        print_usage();
        exit(0);
    }
}

// Track user errors for progressive sass
static struct {
    int error_count;
    char last_error[256];
} UserErrorTracker = {0, ""};

void suggest_similar_option(const char* invalid_option) {
    const char* best_match = NULL;
    int min_distance = INT_MAX;
    
    // Find the most similar valid option
    for (size_t i = 0; i < sizeof(VALID_OPTIONS)/sizeof(ValidOption); i++) {
        int distance = levenshtein_distance(invalid_option, VALID_OPTIONS[i].option);
        if (distance < min_distance && distance <= 3) {  // Max 3 character differences
            min_distance = distance;
            best_match = VALID_OPTIONS[i].option;
        }
    }
    
    if (best_match) {
        printf("💡 Did you mean '%s'?\n", best_match);
        
        // Add contextual hints based on common mistakes
        if (strstr(invalid_option, "grey") || strstr(invalid_option, "gray")) {
            printf("   (Use '-w' for grayscale conversion)\n");
        } else if (strstr(invalid_option, "flip")) {
            printf("   (Use '--fliph' for horizontal or '--flipv' for vertical flips)\n");
        }
    }
}

void handle_progressive_sass(const char* invalid_option) {
    UserErrorTracker.error_count++;
    strncpy(UserErrorTracker.last_error, invalid_option, 255);
    
    printf("\n");
    switch(UserErrorTracker.error_count) {
        case 1:
            printf("😊 Oops! '%s' isn't a valid option.\n", invalid_option);
            break;
        case 2:
            printf("🤔 Still trying with '%s'? Let's check those options again.\n", invalid_option);
            break;
        case 3:
            printf("😅 Third time's... not the charm with '%s'.\n", invalid_option);
            print_simplified_usage();
            break;
        case 4:
            printf("🎮 This isn't a game of option-guessing, but if it were, you'd be losing.\n");
            printf("Here's a super-duper simple guide:\n");
            print_simplified_usage();
            break;
        default:
            printf("🎯 %d attempts and counting! Your persistence is... impressive?\n", 
                   UserErrorTracker.error_count);
            printf("Maybe try:\n");
            printf("1. Reading the manual (--help)\n");
            printf("2. Taking a coffee break (--coffee)\n");
            printf("3. Asking politely (--help-please)\n");
            break;
    }
    
    suggest_similar_option(invalid_option);
}

// Interactive option suggestions
void suggest_next_steps(const char* current_option) {
    printf("\n💡 Common next steps after %s:\n", current_option);
    if (strcmp(current_option, "-w") == 0) {
        printf("   • Add '--sepia' for that vintage look\n");
        printf("   • Try '--contrast 1.2' to make it pop\n");
    } else if (strstr(current_option, "flip") != NULL) {
        printf("   • Add '--rotate 90' for more transformation\n");
        printf("   • Try '--edge' to detect edges\n");
    }
    // ... more contextual suggestions
}

// Update validate_arguments to use new features
void validate_arguments(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "😕 No image file? That's like trying to make a sandwich with no bread!\n\n");
        print_simplified_usage();
        exit(1);
    }

    // Check if help is needed
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage();
            exit(0);
        }
    }

    // Validate input file
    if (access(argv[1], F_OK) != 0) {
        fprintf(stderr, "🔍 File '%s' not found!\n"
                       "   Checked under the keyboard? Behind the monitor?\n"
                       "   No? Maybe try a file that actually exists!\n\n", argv[1]);
        print_simplified_usage();
        exit(1);
    }

    // Validate options
    for (int i = 2; i < argc; i++) {
        // Check for easter eggs first
        handle_easter_egg(argv[i]);
        
        int valid_option = 0;
        for (size_t j = 0; j < sizeof(VALID_OPTIONS)/sizeof(ValidOption); j++) {
            if (strcmp(argv[i], VALID_OPTIONS[j].option) == 0) {
                valid_option = 1;
                suggest_next_steps(argv[i]);
                if (VALID_OPTIONS[j].requires_value) {
                    if (i + 1 >= argc || argv[i + 1][0] == '-') {
                        fprintf(stderr, "😅 Oops! %s\n", VALID_OPTIONS[j].sassy_message);
                        print_simplified_usage();
                        exit(1);
                    }
                    i++; // Skip the value in next iteration
                }
                break;
            }
        }
        if (!valid_option) {
            handle_progressive_sass(argv[i]);
            exit(1);
        }
    }
}

int main(int argc, char *argv[]) {
    // Add some flair
    printf("\n🎨 WhittemoreImageProcessor: Because MS Paint was too mainstream 🎨\n");
    
    // Validate all arguments before processing
    validate_arguments(argc, argv);

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
