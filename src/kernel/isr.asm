bits 32

%macro ISR_NOERRORCODE 1
global ISR%1:
ISR%1:
    push 0                  ; dummy error code
    push %1                 ; push interrupt number
    jmp isr_common
%endmacro

%macro ISR_ERRORCODE 1
global ISR%1:
ISR%1:
                            ; error code is pushed by the cpu
    push %1                 ; push interrupt number
    jmp isr_common
%endmacro

%include "isr_functions.as"


extern ISR_handler
isr_common:
    pusha

    xor eax, eax
    mov ax, ds
    push eax                ; push ds to the stack

    mov ax, 0x10            ; use kernel segments
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp                ; address of stack
    call ISR_handler        ; call the C function
    add esp, 4

    pop eax                 ; restore all the segments
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 8              ; remove the code error
    iret                    ; special interrupt return


