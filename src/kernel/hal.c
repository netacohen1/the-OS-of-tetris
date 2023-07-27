#include "idt.h"
#include "isr.h"
#include "screen.h"
#include "util.h"

void HAL_init(){
    IDT_init();                 // init the IDT
    ISR_init();                 // init the ISR's
    init_serial();              // init the COM1 port (for qemu and bochs)

    write_serial('\n');         // print a new line to seperate the qemu things

}