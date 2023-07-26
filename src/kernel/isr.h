#ifndef ISR_H
#define ISR_H
#include "util.h"

typedef struct {
    // in the reverse order they were pushed to the stack
    u32 ds;                                             // we pushed to the stack
    u32 edi, esi, ebp, cs_kernel, ebx, edx, ecx, eax;   // we pushed with pusha
    u32 interrupt, error;                               // we push interrupt and error is auto (or dummy)
    u32 eip, cs, eflags, esp, ss;                       // pushed auto
} PACKED Registers_t;

typedef void (*ISR_handler)(Registers_t* regs);

void ISR_init();
void CDECL ISR_regsHandler(Registers_t* regs);

#endif
