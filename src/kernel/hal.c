#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "timer.h"
#include "screen.h"
#include "util.h"
#include "keyboard.h"

void HAL_init(){
    IDT_init();                 // init the IDT
    ISR_init();                 // init the ISR's
    IRQ_init();                 // init the IRQ's and pic
    timer_install();            // register timer in irq
    keybrd_install();           // register keyboard in irq

    init_serial();              // init the COM1 port (for qemu and bochs)

    write_serial('\n');         // print a new line to seperate the qemu things

    STI();                      // set interrupts

}
