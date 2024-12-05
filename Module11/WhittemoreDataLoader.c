/**
 * SER334 Page Replacement Algorithm Assignment
 * @author Uriah Whittemore (1224612363)
 * @version 1.0
 *
 * This file implements the data loading functionality for the page
 * replacement simulator, reading test scenarios from input files.
 */

#include "WhittemoreDataLoader.h"

struct test_scenario* load_test_data(char* filename) {
    if (!filename) {
        fprintf(stderr, "Error: Null filename provided\n");
        return NULL;
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return NULL;
    }

    struct test_scenario* scenario = malloc(sizeof(struct test_scenario));
    if (!scenario) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    // Read and validate page count
    if (fscanf(file, "%d", &scenario->page_count) != 1 || scenario->page_count <= 0) {
        fprintf(stderr, "Error: Invalid page count: %d\n", scenario->page_count);
        free(scenario);
        fclose(file);
        return NULL;
    }
    fscanf(file, "%*[^0-9]");

    // Read and validate frame count
    if (fscanf(file, "%d", &scenario->frame_count) != 1 || scenario->frame_count <= 0) {
        fprintf(stderr, "Error: Invalid frame count: %d\n", scenario->frame_count);
        free(scenario);
        fclose(file);
        return NULL;
    }
    fscanf(file, "%*[^0-9]");

    // Read and validate reference string length
    if (fscanf(file, "%d", &scenario->refstr_len) != 1 || 
        scenario->refstr_len <= 0 || 
        scenario->refstr_len > 512) {
        fprintf(stderr, "Error: Invalid reference string length: %d\n", scenario->refstr_len);
        free(scenario);
        fclose(file);
        return NULL;
    }
    fscanf(file, "%*[^0-9]");

    // Read reference string
    for (int i = 0; i < scenario->refstr_len; i++) {
        int result = fscanf(file, "%d", &scenario->refstr[i]);
        if (result != 1) {
            fprintf(stderr, "Error: Failed to read page number at position %d\n", i);
            free(scenario);
            fclose(file);
            return NULL;
        }
        
        if (scenario->refstr[i] < 0 || scenario->refstr[i] >= scenario->page_count) {
            fprintf(stderr, "Error: Invalid page number %d at position %d (must be between 0 and %d)\n", 
                    scenario->refstr[i], i, scenario->page_count - 1);
            free(scenario);
            fclose(file);
            return NULL;
        }
        fscanf(file, "%*[^0-9]");
    }

    fclose(file);
    return scenario;
} 