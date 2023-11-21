#include "util.h"
#include "hal.h"
#include "screen.h"
#include "memory.h"
#include "timer.h"
#include "keyboard.h"


void main(){
    HAL_init();
    set_palette();

    screen_swap();
    clear_screen(COLOR(0,0,0));
    screen_swap();

    while (true){
        if (is_key_pressed('a')){
            clear_screen(COLOR(255,0,0));
            screen_swap();
        }
    }

    while(1){
        __asm__ volatile("hlt");
    }
}
