#include "util.h"
#include "hal.h"
#include "screen.h"
#include "memory.h"
#include "timer.h"


void main(){
    HAL_init();

    set_palette();
    
    for (i16 h=0; h<SCREEN_HEIGHT; h++){
        for (i16 w=0; w<SCREEN_WIDTH; w++){
            plot_pixel(w, h, COLOR(h,255-h,w));
        }
    }
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
