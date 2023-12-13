#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "rect_tools.h"
#include "test_rect.h"

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
    test_rect();
    return 0;
}