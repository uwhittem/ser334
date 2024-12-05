#ifndef WHITTEMORE_PAGETABLE_H
#define WHITTEMORE_PAGETABLE_H

enum replacement_algorithm { FIFO, LRU, MFU };

struct page_table_entry {
    unsigned int bits;        // rightmost bit is valid, second rightmost is dirty
    int frame_number;
    int order;
    int last_accessed;
    int frequency;
};

struct page_table {
    struct page_table_entry *entries;
    int page_count;
    int frame_count;
    enum replacement_algorithm algorithm;
    int verbose;
    int page_faults;
};

struct page_table* page_table_create(int page_count, int frame_count, enum replacement_algorithm algorithm, int verbose);
void page_table_destroy(struct page_table** pt);
void page_table_access_page(struct page_table *pt, int page);
void page_table_display(struct page_table* pt);
void page_table_display_contents(struct page_table *pt);

#endif // WHITTEMORE_PAGETABLE_H