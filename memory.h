#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>

#define PAGE_SIZE 256
#define MAX_FRAMES 256

//Physical Memory
extern signed char physical_mem[MAX_FRAMES][PAGE_SIZE];

//Function Defs
int initialize_memory(void);

int load_page(int page_number, int frame_number, FILE *backing_store);

int read_memory(int frame_number, int offset);

void print_frame(int frame_number);

#endif