#include "pagetable.h"

//Create the Page Table

PageTableEntry page_table[PAGE_COUNT];

//initializing the page table
void initialize_page_table(){
    for (int i = 0; i<PAGE_COUNT;i++){
        page_table[i].frame_num = -1;
        page_table[i].valid = 0;

    };
}

int page_lookup(int page_num){

    //Check if page is within bound
    if (page_num < 0 || page_num >= PAGE_COUNT) return -1;
    //Check if page is valid
    if (page_table[page_num].valid == 1) return page_table[page_num].frame_num;

    //Page is not in memory:
    return -1;
}

void update_page_table(int page_num, int frame_num){
    //check if in bounds
    if (page_num < 0 || page_num >= PAGE_COUNT) return;

    page_table[page_num].frame_num = frame_num;
    page_table[page_num].valid = 1;

}

void invalidate_page(int page_num){
    if (page_num < 0 || page_num >= PAGE_COUNT) return;
    page_table[page_num].frame_num = -1;
    page_table[page_num].valid = 0;
    
}