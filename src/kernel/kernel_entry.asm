[bits    32]
[extern main]

KSTACK_SIZE         equ     64 * 1024   ; stack size 64 KB

extern __end

section .entry
global _start
_start:
    mov esp, __stack                    ; points to bottom of stack
    add esp, KSTACK_SIZE                ; stack grows downwards. now points to top of stack

    ; get mask to align to 16 byte boundry
    mov eax, 0x0f
    not eax

    ; align stack
    and esp, eax

    ; point stack pointer to last valid part of satck
    sub esp, 16

    call main
    hlt

section .bss
__stack:
    times KSTACK_SIZE   db 0
