#ifndef SCREEN_H
#define SCREEN_H
#include "util.h"

#define SCREEN_HEIGHT   200
#define SCREEN_WIDTH    320
#define VIDEO_MEMORY    0xA0000

void set_palette();
void plot_pixel(i32 x, i32 y, u8 color);
u8 COLOR(u8 r, u8 g, u8 b);

#endif
