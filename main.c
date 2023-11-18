#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define PLANE_SIZE 40

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
} rect_t; 

bool is_intersection(rect_t r1, rect_t r2) {
    if ((r2.x + r2.w > r1.x) && (r2.y + r2.h > r1.y))
        if ((r2.x < r1.x + r1.w) && (r2.y < r1.y + r1.h))
            return true;
    if ((r2.x < r1.x + r1.w) && (r2.y + r2.h > r1.y))
        if ((r2.x + r2.w > r1.x) && (r2.y < r1.y + r1.h))
            return true;
    if ((r2.x + r2.w > r1.x) && (r2.y < r1.y + r1.h))
        if ((r2.x < r1.x + r1.w) && (r2.y + r2.h > r1.y))
            return true;
    if ((r2.x < r1.x + r1.w) && (r2.y < r1.y + r1.h))
        if ((r2.x + r2.w > r1.x) && (r2.y + r2.h > r1.y))
            return true;
    return false;
}

bool is_rect_bigger(rect_t src, rect_t substract) {
    if (src.x <= substract.x) {
        if (src.y <= substract.y) {
            return src.x + src.w >= substract.x + substract.w && src.y + src.h >= substract.y + substract.h;
        }
    }
    return false;
}

rect_t resize_if_bigger(rect_t src, rect_t compare) {
    rect_t resized = src;
    if (src.x < compare.x) {
        resized.w -= abs(compare.x - src.x);
        resized.x = compare.x;
    }
    if (src.x + src.w > compare.x + compare.w) {
        resized.w -= abs(compare.x - src.x); 
    }
    if (src.y < compare.y) {
        resized.y = compare.y;
        resized.h = abs(compare.y - compare.h);
    }
    if (src.y + src.h > compare.y + compare.h) {
        resized.h = abs(compare.y - compare.h);
    }
    return resized;
}

void delete_rect(rect_t* src) {
    src->w = 0;
    src->h = 0;
    src->x = 0;
    src->y = 0;
}

rect_t make_up_rect(rect_t src, rect_t substract) {
    return (rect_t){substract.x, src.y, substract.w, abs(src.y - substract.y)};
}

rect_t make_left_rect(rect_t src, rect_t substract) {
    return (rect_t){src.x, src.y, abs(src.x - substract.x),src.h};
}

rect_t make_right_rect(rect_t src, rect_t substract) {
    return (rect_t){substract.x + substract.w, src.y, abs((substract.x + substract.w) - (src.x + src.w)), src.h};
}

rect_t make_down_rect(rect_t src, rect_t substract) {
    return (rect_t){substract.x, substract.y + substract.h, substract.w , abs((substract.y + substract.h) - (src.y + src.h))};
}

bool is_up(rect_t src, rect_t substract) {
    return substract.y != src.y;
}

bool is_down(rect_t src, rect_t substract) {
    return substract.y + substract.h != src.y + src.h;
}

bool is_left(rect_t src, rect_t substract) {
    return substract.x != src.x;
}

bool is_right(rect_t src, rect_t substract) {
    return substract.x + substract.w != src.x + src.w;
}

void rects_substract(rect_t* src, uint8_t src_size, uint8_t* src_cnt, rect_t substract) {
    for (uint8_t i = 0; i < src_size; ++i) {
        if (i > *src_cnt)
            break;
        if (!is_intersection(src[i], substract))
            continue;
        if (is_rect_bigger(substract, src[i])) {
            delete_rect(&src[i]);
            continue;
        }
        rect_t copy = resize_if_bigger(substract, src[i]);
        if (is_up(src[i], copy)) {
            if (is_down(src[i], copy)) {
                src[*src_cnt] = make_down_rect(src[i], copy);
                ++(*src_cnt);
            }
            if (is_left(src[i],copy)) {
                src[*src_cnt] = make_left_rect(src[i], copy);
                ++(*src_cnt);
            }
            if (is_right(src[i], copy)) {
                src[*src_cnt] = make_right_rect(src[i], copy);
                ++(*src_cnt);
            }
            src[i] = make_up_rect(src[i], copy);
            continue;
        }
        if (is_down(src[i],copy)) {
            if (is_left(src[i],copy)) {
                src[*src_cnt] = make_left_rect(src[i], copy);
                ++(*src_cnt);
            }
            if (is_right(src[i], copy)) {
                src[*src_cnt] = make_right_rect(src[i], copy);
                ++(*src_cnt);
            }
            src[i] = make_down_rect(src[i], copy);
            continue;
        }
        if (is_left(src[i],copy)) {
            if (is_right(src[i], copy)) {
                src[*src_cnt] = make_right_rect(src[i], copy);
                ++(*src_cnt);
            }
            src[i] = make_left_rect(src[i], copy);
            continue;
        }
        if (is_right(src[i],copy)) {
            src[i] = make_right_rect(src[i], copy);
            continue;
        }
    }
}

void draw_rectangle(rect_t rect, char xy[][PLANE_SIZE]) {
    for (uint16_t x = rect.x; x < rect.x + rect.w; ++x) {
        for (uint16_t y = rect.y; y < rect.y + rect.h; ++y) {
            xy[y][x] = '*';
        }
    }
}

void draw(char xy[][PLANE_SIZE]) {
    for (uint16_t x = 0; x < 32; ++x) {
        for (uint16_t y = 0; y < 24; ++y) {
            printf("%c", xy[x][y]);
        }
        printf("\n");
    }
}

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