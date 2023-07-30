#ifndef IRQ_H
#define IRQ_H
#include "util.h"
#include "isr.h"

typedef void (*IRQ_Handler)(Registers_t* regs);
void IRQ_init();
void IRQ_RegisterHandler(i32 irq, IRQ_Handler handler);

#endif
