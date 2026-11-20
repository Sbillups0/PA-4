#include "memory.h"

//create memory

signed char physical_mem[MAX_FRAMES][PAGE_SIZE];

//initialize the physical mem

int initialize_memory(){

    for (int i = 0; i < MAX_FRAMES; i++) {
        for (int j = 0; j < PAGE_SIZE; j++) {
            physical_mem[i][j] = 0;
        }
    }

    return 0;
}

//load page into a frame
int load_page(int page_num, int frame_num, FILE *backing_store){

    //Check if page and frame nums are valid
    if (page_num < 0 || page_num >= 256 ||
        frame_num < 0 || frame_num >= MAX_FRAMES) {
        return -1;
    }

    //find start of page
    int position = page_num * 256;

    // Move to the correct position in the backing store
    if (fseek(backing_store, position, SEEK_SET) != 0) {
        return -1;
    }

    size_t bytes_read = fread(physical_mem[frame_num],
        sizeof(signed char),
        PAGE_SIZE,
        backing_store);
    
    if (bytes_read != PAGE_SIZE) {
        return -1;
    }

    return 0;}

    // Reading a byte from physical mem
    int read_memory(int frame_number, int offset) {

    // Check valid frame and offset
    if (frame_number < 0 || frame_number >= MAX_FRAMES ||
        offset < 0 || offset >= PAGE_SIZE) {
        return -1;
    };

    return physical_mem[frame_number][offset];
}

void print_frame(int frame_number) {
    for (int i = 0; i < PAGE_SIZE; i++) {
        printf("%02X", (unsigned char)physical_mem[frame_number][i]);
    }
}