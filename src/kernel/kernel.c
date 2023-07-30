#include "util.h"
#include "hal.h"
#include "screen.h"


void main(){
    HAL_init();

    BP();
    /* __asm__ volatile("int $0x0"); */
    __asm__ volatile("int $0x21");
    BP();

    volatile char* video_memory = (volatile char*)0xA0000;
    set_palette();

    for (i16 h=0; h<SCREEN_HEIGHT; h++){
        for (i16 w=0; w<SCREEN_WIDTH; w++){
            *(video_memory + h*SCREEN_WIDTH + w) = h;
        }
    }
}
