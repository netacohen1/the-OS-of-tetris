#ifndef IDT_H
#define IDT_H
#include "util.h"

#define TRAP_GATE_FLAGS             0x8F    // p=1, dpl=00, 44=0, gate=f
#define INTERRUPT_GATE_FLAGS        0x8E    // p=1, dpl=00, 44=0, gate=e
#define INTERRUPT_USER_GATE_FLAGS   0xEE    // p=1, dpl=11, 44=0, gate=e (called from PL 3)


void IDT_init();
void IDT_set_descriptor(u8 interrupt, void* isr, u8 flags);
void exception_handler();

#endif
