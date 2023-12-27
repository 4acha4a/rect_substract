#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "test_rect.h"
#include "rect_tools.h"

#define MAX_W 65535
#define MAX_H 65535

void test_rect_fuzzy() {
    printf("x; y; w; h\n");
    const uint8_t src_size = 100;
    uint8_t src_cnt = 1;
    rect_t* src = (rect_t*)malloc(src_size * sizeof(rect_t));
    rect_t first_rect = {0, 0, MAX_W, MAX_H};
    src[0] = first_rect;
    const int max_num = MAX_H;
    const int min_num = 0;
    srand(time(0));
    for (size_t i = 0; i < 10; ++i) {
        uint16_t x = rand() % (max_num + 1 - min_num) + min_num;
        uint16_t y = rand() % (max_num + 1 - min_num) + min_num;
        uint16_t w = rand() % (max_num + 1 - min_num) + min_num;
        uint16_t h = rand() % (max_num + 1 - min_num) + min_num;
        rect_t substract = (rect_t){x, y, w, h};
        print_rect(substract);
        rects_substract(src, src_size, &src_cnt, substract);
    }
    printf("\n");
    for (size_t i = 0; i < src_cnt; ++i) {
        print_rect(src[i]);
    }
    free(src);
}

void test_rect_max_count() {
    printf("x; y; w; h\n");
    const uint8_t src_size = 10;
    uint8_t src_cnt = 1;
    rect_t* src = (rect_t*)malloc(src_size * sizeof(rect_t));
    rect_t first_rect = {0, 0, 1000, 1000};
    src[0] = first_rect;

    rect_t s1 = {300, 300, 400, 400};
    rects_substract(src, src_size, &src_cnt, s1);
    print_rect(s1);

    rect_t s2 = {400, 100, 200, 100};
    rects_substract(src, src_size, &src_cnt, s2);
    print_rect(s2);

    rect_t s3 = {400, 800, 200, 100};
    rects_substract(src, src_size, &src_cnt, s3);
    print_rect(s3);

    printf("\n");
    for (size_t i = 0; i < src_cnt; ++i) {
        print_rect(src[i]);
    }
    free(src);
} // PASS

void test_rect_null_rects() {
    printf("x; y; w; h\n");
    const uint8_t src_size = 100;
    uint8_t src_cnt = 1;
    rect_t* src = (rect_t*)malloc(src_size * sizeof(rect_t));
    rect_t first_rect = {0, 0, 1000, 1000};
    src[0] = first_rect;

    rect_t s1 = {300, 300, 400, 400};
    rects_substract(src, src_size, &src_cnt, s1);
    print_rect(s1);

    rect_t s2 = {400, 100, 200, 100};
    rects_substract(src, src_size, &src_cnt, s2);
    print_rect(s2);

    rect_t s3 = {400, 800, 200, 100};
    rects_substract(src, src_size, &src_cnt, s3);
    print_rect(s3);

    rect_t big = {0, 0, 500, 1000};
    rects_substract(src, src_size, &src_cnt, big);
    print_rect(big);

    printf("\n");
    for (size_t i = 0; i < src_cnt; ++i) {
        print_rect(src[i]);
    }
    free(src);
}

void test_rect_hito() {
    printf("x; y; w; h\n");
    const uint8_t src_size = 100;
    uint8_t src_cnt = 1;
    rect_t* src = (rect_t*)malloc(src_size * sizeof(rect_t));
    rect_t hito_screen = {0, 0, 450, 350};
    src[0] = hito_screen;
    rect_t battery = {430, 5, 18, 10};
    rects_substract(src, src_size, &src_cnt, battery);
    print_rect(battery);
    rect_t more = {10, 10, 50, 20};
    rects_substract(src, src_size, &src_cnt, more);
    print_rect(more);
    rect_t etherium_logo = {70, 30, 320, 240};
    rects_substract(src, src_size, &src_cnt, etherium_logo);
    print_rect(etherium_logo);
    rect_t receive_btn = {20, 320, 190, 20};
    rects_substract(src, src_size, &src_cnt, receive_btn);
    print_rect(receive_btn);
    rect_t send_btn = {220, 320, 190, 20};
    rects_substract(src, src_size, &src_cnt, send_btn);
    print_rect(send_btn);
    for (size_t i = 0; i < src_cnt; ++i) {
        print_rect(src[i]);
    }
    free(src);
} // PASS

void test_rect_hito_negative() {
    printf("x; y; w; h\n");
    const uint8_t src_size = 100;
    uint8_t src_cnt = 1;
    rect_t* src = (rect_t*)malloc(src_size * sizeof(rect_t));
    rect_t hito_screen = {0, 0, -450, -350};
    src[0] = hito_screen;
    rect_t battery = {-430, -5, -18, -10};
    rects_substract(src, src_size, &src_cnt, battery);
    print_rect(battery);
    rect_t more = {-10, -10, -50, -20};
    rects_substract(src, src_size, &src_cnt, more);
    print_rect(more);
    rect_t etherium_logo = {-70, -30, -320, -240};
    rects_substract(src, src_size, &src_cnt, etherium_logo);
    print_rect(etherium_logo);
    rect_t receive_btn = {-20, -320, -190, -20};
    rects_substract(src, src_size, &src_cnt, receive_btn);
    print_rect(receive_btn);
    rect_t send_btn = {-220, -320, -190, -20};
    rects_substract(src, src_size, &src_cnt, send_btn);
    print_rect(send_btn);
    for (size_t i = 0; i < src_cnt; ++i) {
        print_rect(src[i]);
    }
    free(src);
} //PASS

void test_rect() {
    // test_rect_null_rects();
    test_rect_fuzzy();
}
