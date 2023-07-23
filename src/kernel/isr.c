#include "isr.h"
#include "util.h"

void ISR_initGates();

void ISR_init(){
    ISR_initGates();
}

void CDECL ISR_handler(Registers_t* registers){
    __asm__ volatile("mov %%eax, %%edx": :"a" (registers->interrupt));
    CLI();
    HLT();
   
}
