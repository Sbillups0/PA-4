#include <string.h>
#include "replacement.h"
#include "memory.h"        // for MAX_FRAMES

#define FIFO 0
#define LRU  1
#define OPT  2

static int algorithm; //Replacement algo in use
static int frame_count;
static int frames_used = 0;
static int fifo_next = 0;
static int page_of[MAX_FRAMES]; //Which page in each frame (-1 is empty)
static int last_used[MAX_FRAMES]; //Last time each frame was used
static int *future; //Future references for OPT algorithm
static int future_len;

int init_replacement(int num_frames, char *alg, int *refs, int num_refs) {
    if (strcmp(alg, "fifo") == 0) algorithm = FIFO;
    else if (strcmp(alg, "lru") == 0) algorithm = LRU;
    else if (strcmp(alg, "opt") == 0) algorithm = OPT;
    else return -1; // Unknown algorithm

    frame_count = num_frames;
    future = refs;
    future_len = num_refs;
    for (int i = 0; i < MAX_FRAMES; i++) {
        page_of[i] = -1;
        last_used[i] = -1;
    }
    return 0;
}

//OPT helper
static int next_use(int page_num, int now) {
    for (int i = now + 1; i < future_len; i++) {
        if (((future[i] >> 8) & 0xFF) == page_num) 
            return i;
    }
    return future_len; // Not found in the future
}

int choose_frame(int now) {
    if(frames_used < frame_count)
        return frames_used++; // Use the next available frame
    //Memory is full: choose a victim
    int victim = 0;
    if (algorithm == FIFO) {
        victim = fifo_next;
        fifo_next = (fifo_next + 1) % frame_count;
    } else if (algorithm == LRU) {
        for (int f = 1; f < frame_count; f++) {
            if (last_used[f] < last_used[victim])
                victim = f;
        }
    } else {
        int farthest = -1;
        for (int f = 0; f < frame_count; f++) {
            int n = next_use(page_of[f], now);
            if (n > farthest) {
                farthest = n;
                victim = f;
            }
        }
    }
    return victim;
}

int page_in_frame(int frame_num) {
    return page_of[frame_num];
}

void frame_loaded(int frame_num, int page_num, int now) {
    page_of[frame_num] = page_num;
    last_used[frame_num] = now;
}

void frame_used(int frame_num, int now) {
    last_used[frame_num] = now;
}