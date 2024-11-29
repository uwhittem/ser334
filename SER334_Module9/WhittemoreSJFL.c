//
// Created by killb on 11/29/2024.
//

#include "WhittemoreSJFL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    int num_ticks, num_processes;
    Process *processes;
    
    // Read input file
    read_input_file(argv[1], &num_ticks, &num_processes, &processes);
    
    // Run both simulations
    printf("\n== Shortest-Job-First ==\n");
    simulate_sjf(processes, num_processes, num_ticks);
    
    // Reset current_tick for all processes
    for (int i = 0; i < num_processes; i++) {
        processes[i].current_tick = 0;
    }
    
    printf("\n== Shortest-Job-First Live ==\n");
    simulate_sjf_live(processes, num_processes, num_ticks);
    
    // Clean up
    free_processes(processes, num_processes);
    
    return 0;
}

void read_input_file(const char *filename, int *num_ticks, int *num_processes, 
                    Process **processes) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file\n");
        exit(1);
    }

    // Read number of ticks and processes
    fscanf(file, "%d", num_ticks);
    fscanf(file, "%*[;\n]");  // Skip semicolon or newline
    fscanf(file, "%d", num_processes);
    fscanf(file, "%*[;\n]");  // Skip semicolon or newline
    
    // Allocate memory for processes
    *processes = malloc(sizeof(Process) * (*num_processes));
    
    // Read process data
    for (int i = 0; i < *num_processes; i++) {
        (*processes)[i].id = i;
        (*processes)[i].current_tick = 0;
        fscanf(file, "%*d");  // Skip process number
        fscanf(file, "%*[;\n]");  // Skip semicolon or newline
        fscanf(file, "%lf", &(*processes)[i].tau);
        fscanf(file, "%*[;\n]");  // Skip semicolon or newline
        fscanf(file, "%lf", &(*processes)[i].alpha);
        fscanf(file, "%*[;\n]");  // Skip semicolon or newline
        
        // Allocate and read burst times
        (*processes)[i].burst_times = malloc(sizeof(int) * (*num_ticks));
        for (int j = 0; j < *num_ticks; j++) {
            fscanf(file, "%d", &(*processes)[i].burst_times[j]);
            fscanf(file, "%*[;\n]");  // Skip semicolon or newline
        }
    }
    
    fclose(file);
}

void simulate_sjf_live(Process *processes, int num_processes, int num_ticks) {
    int current_time = 0;
    int total_turnaround = 0;
    int total_waiting = 0;
    double total_error = 0;

    // Simulate each tick
    for (int tick = 0; tick < num_ticks; tick++) {
        printf("Simulating %dth tick of processes @ time %d:\n", tick, current_time);
        
        // Create array of process indices sorted by tau
        int *order = malloc(sizeof(int) * num_processes);
        for (int i = 0; i < num_processes; i++) {
            order[i] = i;
        }
        
        // Sort by tau values
        for (int i = 0; i < num_processes - 1; i++) {
            for (int j = 0; j < num_processes - i - 1; j++) {
                if (processes[order[j]].tau > processes[order[j + 1]].tau) {
                    int temp = order[j];
                    order[j] = order[j + 1];
                    order[j + 1] = temp;
                }
            }
        }
        
        // Execute processes in order
        int tick_waiting = 0;
        for (int i = 0; i < num_processes; i++) {
            int pid = order[i];
            int actual_burst = processes[pid].burst_times[tick];
            double estimated_burst = processes[pid].tau;
            
            // Calculate estimation error
            total_error += fabs(estimated_burst - actual_burst);
            
            // Update tau for next time
            processes[pid].tau = processes[pid].alpha * actual_burst + 
                               (1 - processes[pid].alpha) * estimated_burst;
            
            // Add waiting time for this process
            total_waiting += tick_waiting;
            
            // Add turnaround time (waiting + burst)
            total_turnaround += (tick_waiting + actual_burst);
            
            printf("Process %d was estimated for %.0f and took %d.\n", 
                   pid, estimated_burst, actual_burst);
            
            // Update waiting time for next process
            tick_waiting += actual_burst;
            current_time += actual_burst;
        }
        
        free(order);
    }
    
    printf("Turnaround time: %d\n", total_turnaround);
    printf("Waiting time: %d\n", total_waiting);
    printf("Estimation Error: %.0f\n", total_error);
}

void simulate_sjf(Process *processes, int num_processes, int num_ticks) {
    int current_time = 0;
    int total_turnaround = 0;
    int total_waiting = 0;

    // Simulate each tick
    for (int tick = 0; tick < num_ticks; tick++) {
        printf("Simulating %dth tick of processes @ time %d:\n", tick, current_time);
        
        // Create array of process indices sorted by burst time
        int *order = malloc(sizeof(int) * num_processes);
        for (int i = 0; i < num_processes; i++) {
            order[i] = i;
        }
        
        // Simple bubble sort by burst time
        for (int i = 0; i < num_processes - 1; i++) {
            for (int j = 0; j < num_processes - i - 1; j++) {
                if (processes[order[j]].burst_times[tick] > 
                    processes[order[j + 1]].burst_times[tick]) {
                    int temp = order[j];
                    order[j] = order[j + 1];
                    order[j + 1] = temp;
                }
            }
        }
        
        // Execute processes in order
        int tick_waiting = 0;
        for (int i = 0; i < num_processes; i++) {
            int pid = order[i];
            int burst = processes[pid].burst_times[tick];
            
            // Add waiting time for this process
            total_waiting += tick_waiting;
            
            // Add turnaround time (waiting + burst)
            total_turnaround += (tick_waiting + burst);
            
            printf("Process %d took %d.\n", pid, burst);
            
            // Update waiting time for next process
            tick_waiting += burst;
            current_time += burst;
        }
        
        free(order);
    }
    
    printf("Turnaround time: %d\n", total_turnaround);
    printf("Waiting time: %d\n", total_waiting);
}

void free_processes(Process *processes, int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        free(processes[i].burst_times);
    }
    free(processes);
}
