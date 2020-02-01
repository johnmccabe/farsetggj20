#pragma once

#include <stdint.h>

#include "32blit.hpp"

void load_assets();
void init();
void update(uint32_t time_ms);
void render(uint32_t time_ms);
void draw_greta(int8_t x, int8_t y);
void draw_logo(int8_t x, int8_t y);

enum
{
    SCREEN_WIDTH = 160,
    SCREEN_HEIGHT = 120,
    BLOCK_WIDTH = 6,
    X_OFFSET = 50
};