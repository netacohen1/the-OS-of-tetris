#include "util.h"
#include "hal.h"
#include "screen.h"
#include "memory.h"
#include "timer.h"
#include "keyboard.h"


void main(){
    HAL_init();
    set_palette();
    QemuPrintf("MAIN FUNCTION");

    screen_swap();
    clear_screen(COLOR(0,0,0));
    screen_swap();
    bool a = is_special_key(DOWN);

    bool swap = false;
    /* clear_screen(COLOR(255,0,0)); */
    while (true){
        if (is_key_pressed('a')){
            clear_screen(COLOR(255,0,0));
            screen_swap();
        }
        if (is_special_key(DOWN)) {
            clear_screen(COLOR(255,0,0));
            /* screen_swap(); */
        }
        else if (is_special_key(UP)) {
            swap = true;
            clear_screen(COLOR(0,255,0));
        }
        else if (is_special_key(LEFT)) {
            swap = true;
            clear_screen(COLOR(0,0,255));
        }
        else if (is_special_key(RIGHT)) {
            swap = true;
            clear_screen(COLOR(255,255,0));
        }
        /* if (swap){ */
        /*     swap = false; */
        /*     screen_swap(); */
        /* } */
    }
    
    /* for (i16 h=0; h<SCREEN_HEIGHT; h++){ */
    /*     for (i16 w=0; w<SCREEN_WIDTH; w++){ */
    /*         plot_pixel(w, h, COLOR(h,255-h,w)); */
    /*     } */
    /* } */
    /* int y=0; */
    /* u64 timer=0; */
    /* while (1){ */
    /*     timer+=6; */
    /*     for (i16 h=0; h<SCREEN_HEIGHT; h++){ */
    /*         for (i16 w=0; w<SCREEN_WIDTH; w++){ */
    /*             plot_pixel(w, h, (h+timer)%256); */
    /*         } */
    /*     } */
    /* } */

    while(1){
        __asm__ volatile("hlt");
    }
}
