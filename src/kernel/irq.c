#include "irq.h"
#include "isr.h"
#include "pic.h"
#include "util.h"

#define PIC_OFFSET 0x20
IRQ_Handler g_IRQHandlers[16];

void IRQ_handler(Registers_t *regs){
    u8 irq = regs->interrupt - PIC_OFFSET;
    
    if (g_IRQHandlers[irq] != 0){
        g_IRQHandlers[irq](regs);
    } else {
        QemuPrintf("Unhandled IRQ %d\n", irq);
    }
    PIC_sendEOI(irq);
}

void IRQ_init(){
    pic_init(PIC_OFFSET, PIC_OFFSET + 8);
    
    // register each hardware interrupt
    for(u8 i=0; i<16; i++){
        ISR_RegisterHandler(PIC_OFFSET + i, IRQ_handler);
    }
}

void IRQ_RegisterHandler(i32 irq, IRQ_Handler handler){
    // register the handler for the specific IRQ
    g_IRQHandlers[irq] = handler;
}
