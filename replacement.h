#ifndef REPLACEMENT_H
#define REPLACEMENT_H



int init_replacement(int num_frames, char *algorithm, int *refs, int num_refs);

int choose_frame(int now);

int page_in_frame(int frame_num);

void frame_loaded(int frame_num, int page_num, int now);

void frame_used(int frame_num, int now);

#endif