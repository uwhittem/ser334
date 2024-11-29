//
// Created by killb on 11/29/2024.
//

#ifndef WHITTEMORESJFL_H
#define WHITTEMORESJFL_H

// Structure to represent a process
typedef struct {
    int id;                 // Process ID
    double tau;            // Estimated next CPU burst
    double alpha;          // Weight for exponential averaging
    int *burst_times;      // Array of actual burst times
    int current_tick;      // Current tick being processed
} Process;

// Function declarations
void read_input_file(const char *filename, int *num_ticks, int *num_processes, 
                    Process **processes);
void simulate_sjf(Process *processes, int num_processes, int num_ticks);
void simulate_sjf_live(Process *processes, int num_processes, int num_ticks);
void free_processes(Process *processes, int num_processes);

#endif //WHITTEMORESJFL_H
