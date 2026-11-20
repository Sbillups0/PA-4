#ifndef TLB_H
#define TLB_H

#define TLB_SIZE 16

void initialize_tlb(void);

int tlb_lookup(int page_num); //returns frame num or -1 on miss

void tlb_add(int page_num, int frame_num); //insert, replacing oldest

void tlb_invalidate(int page_num); //remove page from TLB

#endif