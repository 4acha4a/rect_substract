#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "rect_tools.h"

#define PLANE_SIZE 40

void draw(char xy[][PLANE_SIZE]) {
    for (uint16_t x = 0; x < PLANE_SIZE; ++x) {
        for (uint16_t y = 0; y < PLANE_SIZE; ++y) {
            printf("%c", xy[x][y]);
        }
        printf("\n");
    }
} //Draws a coordinate plane with rectangles on it

void draw_rectangle(rect_t rect, char xy[][PLANE_SIZE]) {
    for (uint16_t x = rect.x; x < rect.x + rect.w; ++x) {
        for (uint16_t y = rect.y; y < rect.y + rect.h; ++y) {
            xy[y][x] = '*';
        }
    }
} //Draws a specific rectangle on a coordinate plane

int main() {
    uint8_t src_size = 100;
    uint8_t src_cnt = 1;
    rect_t* src = (rect_t*)malloc(src_size * sizeof(rect_t));
    src[0] = (rect_t){0, 0, 32, 24};
    char xy[PLANE_SIZE][PLANE_SIZE];
    for (uint16_t x = 0; x < PLANE_SIZE; ++x) {
        for (uint16_t y = 0; y < PLANE_SIZE; ++y) {
            xy[x][y] = '-';
        }
    }
    rect_t initial = src[0];
    for (uint16_t x = initial.x; x <= initial.x + initial.w; ++x) {
        for (uint16_t y = initial.y; y <= initial.y + initial.h; ++y) {
            rects_substract(src, src_size, &src_cnt, (rect_t){x,y,1,1});
        }
    }
    for (uint8_t i = 0; i < src_cnt; ++i) {
        draw_rectangle(src[i], xy);
    }
    draw(xy);
    free(src);
    printf("%d\n", src_cnt);
    return 0;
}