/**
 * SER334 Page Replacement Algorithm Assignment
 * @author Uriah Whittemore (1224612363)
 * @version 1.0
 *
 * This file implements the page table functionality including FIFO,
 * LRU, and MFU page replacement algorithms. It manages page entries,
 * tracks page faults, and handles memory frame allocation.
 */

#include "WhittemorePageTable.h"
#include <stdio.h>
#include <stdlib.h>

// Function declarations for internal use
static int is_valid(struct page_table_entry *entry);
static void set_valid(struct page_table_entry *entry, int valid);
static int find_free_frame(struct page_table *pt);
static int select_victim_page(struct page_table *pt);

// Required function implementations
struct page_table* page_table_create(int page_count, int frame_count, enum replacement_algorithm algorithm, int verbose) {
    struct page_table* pt = malloc(sizeof(struct page_table));
    pt->entries = malloc(sizeof(struct page_table_entry) * page_count);
    pt->page_count = page_count;
    pt->frame_count = frame_count;
    pt->algorithm = algorithm;
    pt->verbose = verbose;
    pt->page_faults = 0;

    // Initialize entries
    for (int i = 0; i < page_count; i++) {
        pt->entries[i].bits = 0;
        pt->entries[i].frame_number = -1;
        pt->entries[i].order = 0;
        pt->entries[i].last_accessed = 0;
        pt->entries[i].frequency = 0;
    }

    return pt;
}

void page_table_destroy(struct page_table** pt) {
    free((*pt)->entries);
    free(*pt);
    *pt = NULL;
}

void page_table_access_page(struct page_table *pt, int page) {
    static int time_counter = 0;
    time_counter++;
    
    struct page_table_entry *entry = &pt->entries[page];
    entry->last_accessed = time_counter;
    entry->frequency++;
    
    if (!is_valid(entry)) {
        pt->page_faults++;
        
        if (pt->verbose) {
            printf("Page fault occurred for page %d\n", page);
        }

        int free_frame = find_free_frame(pt);
        if (free_frame != -1) {
            entry->frame_number = free_frame;
            set_valid(entry, 1);
            entry->order = time_counter;
            
            if (pt->verbose) {
                printf("Assigned free frame %d to page %d\n", free_frame, page);
            }
        } else {
            int victim_page = select_victim_page(pt);
            struct page_table_entry *victim = &pt->entries[victim_page];
            
            if (pt->verbose) {
                printf("Replacing page %d in frame %d with page %d\n", 
                       victim_page, victim->frame_number, page);
            }

            entry->frame_number = victim->frame_number;
            set_valid(entry, 1);
            set_valid(victim, 0);
            entry->order = time_counter;
        }
    } else if (pt->verbose) {
        printf("Page %d already in memory, frame %d\n", page, entry->frame_number);
    }
}

void page_table_display(struct page_table* pt) {
    printf("==== Page Table ====\n");
    switch(pt->algorithm) {
        case FIFO:
            printf("Mode : FIFO\n");
            break;
        case LRU:
            printf("Mode : LRU\n");
            break;
        case MFU:
            printf("Mode : MFU\n");
            break;
    }
    printf("Page Faults : %d\n", pt->page_faults);
    page_table_display_contents(pt);
}

void page_table_display_contents(struct page_table *pt) {
    if (pt->verbose) {
        printf("page frame | dirty valid | order last freq\n");
        for (int i = 0; i < pt->page_count; i++) {
            struct page_table_entry *entry = &pt->entries[i];
            int dirty_bit = (entry->bits >> 1) & 1;
            int valid_bit = entry->bits & 1;
            printf("%-4d %-5d | %-5d %-5d | %-5d %-4d %-4d\n", 
                   i, entry->frame_number, 
                   dirty_bit, valid_bit, 
                   entry->order, entry->last_accessed, entry->frequency);
        }
    } else {
        printf("page frame | dirty valid\n");
        for (int i = 0; i < pt->page_count; i++) {
            struct page_table_entry *entry = &pt->entries[i];
            int dirty_bit = (entry->bits >> 1) & 1;
            int valid_bit = entry->bits & 1;
            printf("%-4d %-5d | %-5d %-5d\n", 
                   i, entry->frame_number, 
                   dirty_bit, valid_bit);
        }
    }
}

// Helper function implementations
static int is_valid(struct page_table_entry *entry) {
    return entry->bits & 1;
}

static void set_valid(struct page_table_entry *entry, int valid) {
    if (valid) {
        entry->bits |= 1;
    } else {
        entry->bits &= ~1;
    }
}

static int find_free_frame(struct page_table *pt) {
    int used_frames[pt->frame_count];
    for (int i = 0; i < pt->frame_count; i++) {
        used_frames[i] = 0;
    }
    
    for (int i = 0; i < pt->page_count; i++) {
        if (is_valid(&pt->entries[i])) {
            used_frames[pt->entries[i].frame_number] = 1;
        }
    }
    
    for (int i = 0; i < pt->frame_count; i++) {
        if (!used_frames[i]) {
            return i;
        }
    }
    return -1;
}

static int select_victim_page(struct page_table *pt) {
    int victim = -1;
    
    for (int i = 0; i < pt->page_count; i++) {
        if (is_valid(&pt->entries[i])) {
            if (victim == -1) {
                victim = i;
            } else {
                switch(pt->algorithm) {
                    case FIFO:
                        if (pt->entries[i].order < pt->entries[victim].order) {
                            victim = i;
                        }
                        break;
                    case LRU:
                        if (pt->entries[i].last_accessed < pt->entries[victim].last_accessed) {
                            victim = i;
                        }
                        break;
                    case MFU:
                        if (pt->entries[i].frequency > pt->entries[victim].frequency ||
                            (pt->entries[i].frequency == pt->entries[victim].frequency && i < victim)) {
                            victim = i;
                        }
                        break;
                }
            }
        }
    }
    
    return victim;
}

  