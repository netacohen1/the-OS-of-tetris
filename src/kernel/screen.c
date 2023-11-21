#include "util.h"
#include "memory.h"
#include "screen.h"

static u8 *BUFFER = (u8*) 0xA0000;

u8 buffers[2][SCREEN_SIZE];
u8 current_buffer = 0;

#define CURRENT     (buffers[current_buffer])
#define SWAP()      (current_buffer = 1 - current_buffer)

#define VGA_MASK        0x3c6
#define VGA_READ_ADD    0x3c7
#define VGA_WRITE_ADD   0x3c8
#define VGA_DATA        0x3c9

void screen_swap(){
    memcpy(BUFFER, CURRENT, SCREEN_SIZE);
    SWAP();
}

void clear_screen(u8 color){
    memset(&CURRENT, color, SCREEN_SIZE);
}

void plot_pixel(i32 x, i32 y, u8 color){
    volatile u8* pos = CURRENT + SCREEN_WIDTH * y + x;
    *pos = color;
}

void set_palette(){
    outb(VGA_WRITE_ADD, 0);
    for(u8 i=0; i < 255; i++){
        // we shift it by 2 because each color is 6 bits
        outb(VGA_DATA, ((i >> 5) & 0x07) * (256 / 8) >> 2);
        outb(VGA_DATA, ((i >> 2) & 0x07) * (256 / 8) >> 2);
        outb(VGA_DATA, ((i >> 0) & 0x03) * (256 / 4) >> 2);
    }
    // (255, 255, 255) - is white 
    outb(VGA_WRITE_ADD, 255);
    // same reason for the shift as above
    outb(VGA_DATA, 0xff >> 2);
    outb(VGA_DATA, 0xff >> 2);
    outb(VGA_DATA, 0xff >> 2);
}

u8 COLOR(u8 r, u8 g, u8 b){
    // for each color we take the 3 (or 2) MSB and combine them to a number between 0-255
    u8 color = 0;
    color |= ((b / (256 / 4)) & 0x03) << 0;
    color |= ((g / (256 / 8)) & 0x07) << 2;
    color |= ((r / (256 / 8)) & 0x07) << 5;
    return color;
}
