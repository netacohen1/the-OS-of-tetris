#include "isr.h"
#include "util.h"

ISR_handler ISR_handlerArray[256];

i8* g_Exceptions[] = {
    "Division Error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "",
    "",
    "",
    "",
    "",
    "",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    ""
};

void ISR_initGates();
void ISR_init(){
    ISR_initGates();
}

void CDECL ISR_regsHandler(Registers_t* regs){
    if (ISR_handlerArray[regs->interrupt]){
        ISR_handlerArray[regs->interrupt](regs);

    } else if (regs->interrupt >= 32) { 
        QemuPrintf("Unhandeled interrupt %d --> 0x%x\n", regs->interrupt, regs->interrupt);
    } else {
        QemuPrintf("Unhandeled exception %x: ", regs->interrupt);
        QemuPrintf("%s\n", g_Exceptions[regs->interrupt]);

        QemuPrintf("Registers values: \neax->%x ebx->%x ecx->%x edx->%x ebp->%x esi->%x edi->%x\n", regs->eax, regs->ebx, regs->ecx, regs->edx, regs->ebp, regs->esi, regs->edi);
        QemuPrintf("eip->%x cs->%x eflags->%x esp->%x ss->%x\n", regs->eip, regs->cs, regs->eflags, regs->esp, regs->ss);

        QemuPrintf("Interrupt->%x, error_code->%x\n", regs->interrupt, regs->error);
        QemuPrintf("KERNEL PANIC!!\n\n");
        CLI();
        HLT();
    }
}
