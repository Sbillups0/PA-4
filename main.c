#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "tlb.h"
#include "pagetable.h"
#include "replacement.h"


int main(int argc, char *argv[]) {
    //Initial command line argument checks
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <backing_store_file>\n", argv[0]);
        return 1;
    }

    int frames = 256;
    char *pra = "fifo";
    if (argc >= 3) {
        frames = atoi(argv[2]);
    }
    if (argc >= 4) {
        pra = argv[3];
    }
    
    if (frames <= 0 || frames > 256) {
        fprintf(stderr, "Invalid number of frames. Must be between 1 and 256.\n");
        return 1;
    }

    //Open the logical address file for reading
    FILE *addr_file = fopen(argv[1], "r");
    if (addr_file == NULL) {
        perror(argv[1]);
        return 1;
    }
    //Open the backing store file for reading
    FILE *backing_store = fopen("BACKING_STORE.bin", "rb");
    if (backing_store == NULL) {
        perror("BACKING_STORE.bin");
        fclose(addr_file);
        return 1;
    }
    // Read all address into array (for OPT algo)
    int *addresses = NULL;
    int count = 0; // Number of addresses stored
    int capacity = 0;
    int address;
    while(fscanf(addr_file, "%d", &address) == 1) {
        if (count == capacity) {
            // Increase the capacity of the addresses array
            capacity = (capacity == 0) ? 64 : capacity * 2;
            addresses = realloc(addresses, capacity * sizeof(int));
            if (addresses == NULL) {
                perror("realloc");
                return 1;
            }
        }
        addresses[count++] = address;
    }
    fclose(addr_file);

    // Set everything up for the replacement algorithm
    initialize_memory();
    initialize_page_table();
    initialize_tlb();
    if (init_replacement(frames, pra, addresses, count) < 0) {
        fprintf(stderr, "PRA must be fifo, lru, or opt\n");
        return 1;
    }

    int page_faults = 0;
    int tlb_hits = 0;
    int tlb_misses = 0;
    
    for (int i = 0; i < count; i++) {
        address = addresses[i];
        int page_num = (address >> 8) & 0xFF; //bits 15-8
        int offset = address & 0xFF; //bits 7-0
        int frame_num = tlb_lookup(page_num);
        if (frame_num != -1) {
            tlb_hits++;
        } else {
            //TLB miss handling
            tlb_misses++;
            frame_num = page_lookup(page_num);
            if (frame_num == -1) {
                //Page fault handling
                page_faults++;
                frame_num = choose_frame(i);
                int old_page = page_in_frame(frame_num);
                if (old_page != -1) {
                    invalidate_page(old_page);
                    tlb_invalidate(old_page);
                }

                load_page(page_num, frame_num, backing_store);
                update_page_table(page_num, frame_num);
                frame_loaded(frame_num, page_num, i);
            }
            tlb_add(page_num, frame_num);
        }
        frame_used(frame_num, i);
        int value = read_memory(frame_num, offset);
        printf("%d, %d, %d, ", address, value, frame_num);
        print_frame(frame_num);
        printf("\n");
    }
    //Print summary statistics
    printf("Number of Translated Addresses = %d\n", count);
    printf("Page Faults = %d\n", page_faults);
    printf("Page Fault Rate = %.3f\n", (double)page_faults / count);
    printf("TLB Hits = %d\n", tlb_hits);
    printf("TLB Misses = %d\n", tlb_misses);
    printf("TLB Hit Rate = %.3f\n", (double)tlb_hits / count);

    free(addresses);
    fclose(backing_store);
    return 0;
}