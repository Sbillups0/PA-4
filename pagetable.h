#ifndef PAGETABLE_H
#define PAGETABLE_H

#define PAGE_COUNT 256

//Struct for each Pagetable Entry
typedef struct {
    int frame_num;
    int valid; // 0 is invalid, 1 is valid; can change if you'd like
} PageTableEntry;

//Function Definitions
void initialize_page_table();

int page_lookup(int page_num);

void update_page_table(int page_num, int frame_num);

void invalidate_page(int page_num);
#endif