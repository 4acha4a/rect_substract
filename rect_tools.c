#include "rect_tools.h"

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