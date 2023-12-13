#ifndef RECT_TOOLS_H
#define RECT_TOOLS_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
} rect_t; 

void rects_substract(rect_t* src, uint8_t src_size, uint8_t* src_cnt, rect_t substract); //Substract src rectangle from substract, the result is written to *src and *src_cnt
void print_rect(rect_t rect);
bool is_null_rect(rect_t rect);

#endif