#ifndef SCREEN_H
#define SCREEN_H
#include "util.h"

#define SCREEN_HEIGHT   200
#define SCREEN_WIDTH    320
#define SCREEN_SIZE     (SCREEN_HEIGHT*SCREEN_WIDTH) 

void set_palette();
void plot_pixel(u32 x, u32 y, u8 color);
void screen_swap();
void clear_screen(u8 color);
u8 COLOR(u8 r, u8 g, u8 b);

#endif
