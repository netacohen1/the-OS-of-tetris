[bits 32]

; void CDECL IDT_load(idtr_32* idtr);
global IDT_load
IDT_load:
    ; make new call frame
    push ebp            ; save old call frame
    mov ebp, esp        ; make new call frame

    ; load the idtr
    mov eax, [ebp + 8]
    lidt [eax]

    mov esp, ebp        ; restore old call frame
    pop ebp
    ret
