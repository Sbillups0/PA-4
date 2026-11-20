#include "tlb.h"

//Struct for each TLB slot, contains page, frame, and valid flag
typedef struct {
    int page_num;
    int frame_num;
    int valid; // 0 is empty slot, 1 is valid
} TLBEntry;

static TLBEntry tlb[TLB_SIZE];
static int next_slot = 0; // points to where new entry should be inserted (oldest entry replacement)

void initialize_tlb(void) {
    for (int i = 0; i < TLB_SIZE; i++){
        tlb[i].valid = 0; // initialize all slots as empty
    }
    next_slot = 0; 
}

//Search slots for page number, returning frame number if found, -1 if not found
int tlb_lookup(int page_num) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].valid && tlb[i].page_num == page_num) {
            return tlb[i].frame_num; //TLB hit
        }
    }
    return -1; // TLB miss
}

//Insert new entry into TLB
void tlb_add(int page_num, int frame_num) {
    tlb[next_slot].page_num = page_num;
    tlb[next_slot].frame_num = frame_num;
    tlb[next_slot].valid = 1; // mark as valid
    next_slot = (next_slot + 1) % TLB_SIZE; // move to next slot for replacement
}

void tlb_invalidate(int page_num) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if(tlb[i].valid && tlb[i].page_num == page_num) {
            tlb[i].valid = 0; // invalidate the entry
        }
    }
}
