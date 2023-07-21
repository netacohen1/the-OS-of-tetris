#include "util.h"

#define VGA_MASK        0x3c6
#define VGA_READ_ADD    0x3c7
#define VGA_WRITE_ADD   0x3c8
#define VGA_DATA        0x3c9

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
