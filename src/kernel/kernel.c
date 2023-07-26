#include "screen.h"
#include "util.h"
#include "idt.h"


void main(){
    IDT_init();
    init_serial();

    volatile char* video_memory = (volatile char*)0xA0000;
    set_palette();

    for (i16 h=0; h<SCREEN_HEIGHT; h++){
        for (i16 w=0; w<SCREEN_WIDTH; w++){
            *(video_memory + h*SCREEN_WIDTH + w) = h;
        }
    }
}
