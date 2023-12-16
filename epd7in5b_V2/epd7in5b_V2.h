/**
 * @file epd7in5b_V2.h
 * @brief 7.5inch EPD driver
 * @version 1.0
 * @date 2023-10-03
 * 
 * @author Sidpatchy
 */

#ifndef EPD7IN5B_V2_H
#define EPD7IN5B_V2_H
#include <stdint.h>

// Display resolution
constexpr int display_width = 800;
constexpr int display_height = 480;

class EPD7IN5B_V2 {
public:
    static void init();
    static void init180();
    static void clearWhite();
    static void clearBlack();
    static void clearRed();
//    static void show3Block();
    static void display(const uint8_t *blackimage, const uint8_t *ryimage);
    static void enterDeepSleep();
};

#endif
