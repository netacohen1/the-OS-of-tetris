#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include "util.h"

#define TRAP_GATE_FLAGS             0x8F    // p=1, dpl=00, 44=0, gate=f
#define INTERRUPT_GATE_FLAGS        0x8E    // p=1, dpl=00, 44=0, gate=e
#define INTERRUPT_USER_GATE_FLAGS   0xEE    // p=1, dpl=00, 44=0, gate=e (called from PL 3)

/* void idt_set_descriptor(u8 entry_number, void* isr, u8 flags); */
/* void set_IDT(void); */
void IDT_init();
void IDT_disable_gate(int interrupt);
void IDT_enable_gate(int interrupt);
void IDT_set_gate(int interrupt, void* base, u16 segmentDescriptor, u8 flags);




#endif


