#include "idt.h"
#include "util.h"

typedef struct {
    u16     isr_low;            // lower 16 bit of ISR's address
    u16     kernel_cs;          // gdt segment selector
    u8      reserved;           // set to zero
    u8      attribute;          // attribute flags
    u16     isr_high;           // higher 16 bit of ISR's address
    
} PACKED idt_entry_32;

// IDTR layout
typedef struct{
    u16             limit;
    idt_entry_32*   base;
} PACKED idtr_32;

idt_entry_32 idt[256];
idtr_32 idtr = {sizeof(idt)-1, idt};


void IDT_set_gate(int interrupt, void (*base)(), u16 segmentDescriptor, u8 flags){
    idt[interrupt].isr_low = (u32)base & 0xFFFF;
    idt[interrupt].kernel_cs = segmentDescriptor;
    idt[interrupt].reserved = 0;
    idt[interrupt].attribute = flags;
    idt[interrupt].isr_high = ((u32)base >> 16) & 0xFFFF;
}

void IDT_enable_gate(int interrupt){
    SETFLAG(idt[interrupt].attribute, 0x80);        // 0x80 = enabled gate
}

void IDT_disable_gate(int interrupt){
    UNSETFLAG(idt[interrupt].attribute, 0x80);
}

void CDECL IDT_load(idtr_32* idtr);

void IDT_init(){
    IDT_load(&idtr);
}
