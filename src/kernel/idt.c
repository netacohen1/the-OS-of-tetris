#include "idt.h"
#include "util.h"

typedef struct {
    u16     isr_low;            // lower 16 bit of ISR's address
    u16     kernel_cs;          // gdt segment selector
    u8      reserved;           // set to zero
    u8      flags;              // attribute flags
    u16     isr_high;           // higher 16 bit of ISR's address
} PACKED idt_entry_t;

typedef struct {
    u16     limit;
    u32     ptr;
} PACKED idtr_t;

__attribute__((aligned(0x10))) static idt_entry_t idt[256];
static idtr_t idtr;


void exception_handler(){
    // TODO: change this to something better
    volatile char* video_memory = (volatile char*)0xA0000;
    *video_memory = 7;
   /* __asm__ volatile("cli; hlt"); */
}

void IDT_set_descriptor(u8 interrupt, void* isr, u8 flags){
    idt_entry_t* descriptor = &idt[interrupt];

    descriptor->isr_low     = (u32)isr & 0xFFFF;
    descriptor->kernel_cs   = 0x08;
    descriptor->reserved    = 0;
    descriptor->flags       = flags;
    descriptor->isr_high    = ((u32)isr >> 16) & 0xFFFF;
}

extern void* isr_stub_table[];
void IDT_init(){
    idtr.ptr = (u32)&idt[0];
    idtr.limit = (u16)sizeof(idt) - 1;

    for(u8 interrupt=0; interrupt<32; interrupt++){
        IDT_set_descriptor(interrupt, isr_stub_table[interrupt], TRAP_GATE_FLAGS);
    }

    __asm__ volatile("lidt %0": :"m"(idtr));
    STI();
}












