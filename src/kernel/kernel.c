#include "util.h"
#include "hal.h"
#include "screen.h"
#include "memory.h"
#include "timer.h"
#include "keyboard.h"


void main(){
    HAL_init();
    set_palette();

    clear_screen(COLOR(0,0,0));
    int x = 100, y = 100;
    plot_rect(250, 100, 100, 100, COLOR(255,0,0));
    screen_swap();

    while (true){
        switch (getPressed()) {
            case KEY_RIGHT:
                x += 5;
                break;
            case KEY_LEFT:
                x -= 5;
                break;
            case KEY_UP:
                y -= 5;
                break;
            case KEY_DOWN:
                y += 5;
                break;
        }
        clear_screen(COLOR(0,0,0));
        plot_rect(x, y, 90, 100, COLOR(255,0,0));
        screen_swap();
    }

    while(1){
        __asm__ volatile("hlt");
    }
}
