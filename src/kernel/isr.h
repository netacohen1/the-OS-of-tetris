#ifndef ISR_H
#define ISR_H
#include "util.h"

typedef struct {
    // in the reverse order they were pushed to the stack
    u32 ds;                                                 // we pushed to the stack
    u32 eax, ecx, edx, ebx, kernel_esp, ebp, esi, edi;      // we pushed with pusha
    u32 interrupt, error;                                   // we push interrupt and error is auto (or dummy)
    u32 eip, cs, eflags, esp, sp, ss;                       // pushed auto
} PACKED Registers_t;

void ISR_init();

#endif
