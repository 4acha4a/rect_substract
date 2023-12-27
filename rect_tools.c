#include "rect_tools.h"
#include <stdio.h>
#include <errno.h>

extern int errno;

#define NULL_RECT (rect_t){0, 0, 0, 0}

void is_buffer_full(uint8_t src_cnt, uint8_t src_size) {
    if (src_cnt >= src_size) {
        errno = 12;
        perror("Error adding rectangles");
        exit(errno);
    }
} // Stops execution of the program in case the buffer is full

size_t find_null_rect_index(rect_t* src) {
    size_t index = 0;
    while (1) {
        if (is_null_rect(src[index]))
            break;
        ++index;
    }
    return index;
} // Finds the index of a null rect

void rearrange(rect_t* src, uint8_t* src_cnt) {
    for (size_t i = find_null_rect_index(src); i < *src_cnt; ++i) {
        rect_t temp = src[i];
        src[i] = src[i + 1];
        src[i + 1] = temp;
    }
    --(*src_cnt);
} // Rearranges the src array so that the src[*src_cnt] == NULL_RECT

void print_rect(rect_t rect) {
    printf("%d; %d; %d; %d\n", rect.x, rect.y, rect.w, rect.h);
} // Prints rect to console in CSV format

bool is_null_rect(rect_t rect) {
    return rect.x == 0 && rect.y == 0 && rect.w == 0 && rect.h == 0;
} // Checks if rect is a null_rect ({0, 0, 0 ,0})

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
} // Checks if rectangles intersect one another

bool is_rect_inside(rect_t src, rect_t substract) {
    if (src.x <= substract.x) {
        if (src.y <= substract.y) {
            return src.x + src.w >= substract.x + substract.w && src.y + src.h >= substract.y + substract.h;
        }
    }
    return false;
} // Checks if one rectangle is fully inside another


rect_t resize_if_bigger(rect_t src, rect_t compare) {
    rect_t resized = src;
    if (src.x < compare.x) {
        resized.w -= abs(compare.x - src.x);
        resized.x = compare.x;
    }
    if (src.x + src.w > compare.x + compare.w) {
        resized.w -= abs((compare.x + compare.w) - (src.x + src.w)); 
    }
    if (src.y < compare.y) {
        resized.h -= abs(compare.y - src.y);
        resized.y = compare.y;
    }
    if (src.y + src.h > compare.y + compare.h) {
        resized.h -= abs((compare.y + compare.h) - (src.y + src.h)); 
    }
    return resized;
} // In order to substract one rectangle from another, substracted rect must be resized if one of it's sides is out of src rect

void delete_rect(rect_t* src) {
    src->w = 0;
    src->h = 0;
    src->x = 0;
    src->y = 0;
} // Deletes a rectangle (makes it a NULL_RECT)

rect_t make_up_rect(rect_t src, rect_t substract) {
    return (rect_t){substract.x, src.y, substract.w, abs(src.y - substract.y)};
} // Creates upper rectangle

rect_t make_left_rect(rect_t src, rect_t substract) {
    return (rect_t){src.x, src.y, abs(src.x - substract.x),src.h};
} // Creates lower rectangle

rect_t make_right_rect(rect_t src, rect_t substract) {
    return (rect_t){substract.x + substract.w, src.y, abs((substract.x + substract.w) - (src.x + src.w)), src.h};
} // Creates left rectangle

rect_t make_down_rect(rect_t src, rect_t substract) {
    return (rect_t){substract.x, substract.y + substract.h, substract.w , abs((substract.y + substract.h) - (src.y + src.h))};
} // Creates right rectangle

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
    is_buffer_full(*src_cnt, src_size); // Each time we check if buffer is full and if it is, we stop the execution of the program
    if (is_null_rect(substract)) {
        return;
    }
    for (uint8_t i = 0; i < src_size; ++i) {
        if (i >= *src_cnt)
            break;
        if (is_null_rect(src[i]))
            continue;
        if (!is_intersection(src[i], substract))
            continue; // If rectangles don't intersect we go on
        if (is_rect_inside(substract, src[i])) {
            delete_rect(&src[i]);
            rearrange(src, src_cnt);
            --i; 
            continue;
        } // If substracted rectangles is bigger than the source one, the last is deleted, the list is rearranged and we go on
        rect_t copy = resize_if_bigger(substract, src[i]);
        if (is_up(src[i], copy)) {
            if (is_down(src[i], copy)) {
                src[*src_cnt] = make_down_rect(src[i], copy);
                ++(*src_cnt);
                is_buffer_full(*src_cnt, src_size); // Check if the buffer is full
            }
            if (is_left(src[i],copy)) {
                src[*src_cnt] = make_left_rect(src[i], copy);
                ++(*src_cnt);
                is_buffer_full(*src_cnt, src_size); // Check if the buffer is full       
            }
            if (is_right(src[i], copy)) {
                src[*src_cnt] = make_right_rect(src[i], copy);
                ++(*src_cnt);
                is_buffer_full(*src_cnt, src_size); // Check if the buffer is full            
            }
            src[i] = make_up_rect(src[i], copy);
            continue;
        }
        if (is_down(src[i],copy)) {
            if (is_left(src[i],copy)) {
                src[*src_cnt] = make_left_rect(src[i], copy);
                ++(*src_cnt);
                is_buffer_full(*src_cnt, src_size); // Check if the buffer is full          
            }
            if (is_right(src[i], copy)) {
                src[*src_cnt] = make_right_rect(src[i], copy);
                ++(*src_cnt);
                is_buffer_full(*src_cnt, src_size); // Check if the buffer is full                 
            }
            src[i] = make_down_rect(src[i], copy);
            continue;
        }
        if (is_left(src[i],copy)) {
            if (is_right(src[i], copy)) {
                src[*src_cnt] = make_right_rect(src[i], copy);
                ++(*src_cnt);
                is_buffer_full(*src_cnt, src_size); // Check if the buffer is full             
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