org      0x7c00
bits     16

%define ENDL 0x0D, 0x0a

start:
	; setup data segments
	mov ax, 0
	mov ds, ax
	mov es, ax
    
	; setup stack
    mov ss, ax
    mov sp, 0x7C00              ; stack grows downwards from where we are loaded in memory
	
	; some bioses might start us in 07c0:0000 instead of 0000:7c00, so we fix this
	push es
	push word .after
	retf

.after:
    mov [BOOT_DRIVE], dl

    ; mov si, msg_hello
    ; call puts

    call load_kernel

    ; mov si, msg_disk_success
    ; call puts

    call changeVideoMode        ; set the video mode and clear the screen

;----------32 bit change----------
    cll
    call EnableA20   
    call LoadGDT

    mov eax, cr0
    or eax, 0x01
    mov cr0, eax

    jmp dword 0x08:jmpToKernel

bits 32
jmpToKernel:
    cli

    mov ax, 0x10
    mov es, ax
    mov ds, ax
    mov ss, ax

    call KERNEL_OFFSET
    hlt

bits 16
EnableA20:
    ; disable keyboard
    call A20WaitInput
    mov al, kbdDisable
    out kbdCommand, al

    ; read controll port output
    call A20WaitInput
    mov al, kbdRead
    out kbdCommand, al

    call A20WaitOutput
    in al, kbdData
    push eax

    ; write control output port
    call A20WaitInput
    mov al, kbdWrite
    out kbdCommand, al

    call A20WaitInput
    pop eax
    or eax, 2
    out kbdData, al

    ; enable keyboard
    call A20WaitInput
    mov al, kbdEnable
    out kbdCommand, al

    call A20WaitInput
    ret

bits 16
A20WaitInput:
    ; wait until status bit 2 is 0
    in al, kbdCommand
    test al, 2
    jnz A20WaitInput
    ret

bits 16
A20WaitOutput:
    ; wait until status bit 1 is 1
    in al, kbdCommand
    test al, 1
    jz A20WaitOutput
    ret

bits 16
LoadGDT:
    lgdt [g_GDTDesc]
    ret

bits 16
changeVideoMode:
    push ax

    mov ah, 0x00
    mov al, 0x13
    int 0x10

    pop ax
    ret

bits 16
load_kernel:
    ; loads the kernel from memory
    push bx
    push dx

    mov bx, KERNEL_OFFSET
    mov dh, 0xa             ;number of sectors to read
    mov dl, [BOOT_DRIVE]
    call disk_load

    pop dx
    pop bx
    ret

bits 16
disk_load:
    push ax
    push bx
    push cx
    push dx
    push di

    mov ah, 02h     ; mode, 2=read from disk
    mov al, dh      ; num of sectors to read
    mov cl, 02h     ; start from sector

    mov ch, 0x00    ; cylinder num
    mov dh, 0x00    ; head num

    mov di, 3       ; retry count

.loop:
    pusha
    stc             ; set carry

    int 0x013
    jnc .done

    ; read failed
.fail:
    popa
    call disk_reset

    dec di
    test di, di
    jnz .loop
    jmp floppy_error

.done:
    popa

    pop di
    pop dx
    pop cx
    pop bx
    pop ax
    ret
    
;
; Resets disk controller
; Parameters:
;   dl: drive number
;
disk_reset:
    pusha
    mov ah, 0
    stc
    int 13h
    jc floppy_error
    popa
    ret

floppy_error:
    mov si, msg_disk_error
    call puts
    call wait_key_and_reboot

puts:
    ; prints a string from si to the screen
    push si
    push ax
    push bx

.loop:
    lodsb
    or al, al
    jz .done

    mov ah, 0x0e
    mov bh, 0       ; set page number
    int 0x10
    jmp .loop

.done:
    pop bx
    pop ax
    pop si
    ret

wait_key_and_reboot:
    mov ah, 0
    int 16h                     ; wait for keypress
    jmp 0FFFFh:0                ; jump to beginning of BIOS, should reboot

g_GDT:
    ; NULL descrytpor
    dq 0

    ; 32 code segment
code_seg:
    dw 0FFFFh                   ; limit
    dw 0                        ; base
    db 0                        ; base
    db 10011010b                ; access
    db 11001111b                ; granularity
    db 0                        ; base high

    ; 32 data segment
data_seg:
    dw 0FFFFh                   ; limit
    dw 0                        ; base
    db 0                        ; base
    db 10010010b                ; access
    db 11001111b                ; granularity
    db 0                        ; base high

    ; ; 32 system segment
    ; dw 0FFFFh                   ; limit
    ; dw 0                        ; base
    ; db 0                        ; base
    ; db 10001010b                ; access
    ; db 11001111b                ; granularity
    ; db 0                        ; base high

g_GDTDesc:
    dw g_GDTDesc - g_GDT - 1    ; size of gdt
    dd g_GDT                    ; address of gdt

BOOT_DRIVE db 0             ; boot drive variable

msg_hello: db "hello there!", ENDL, 0
msg_disk_error: db "Error in disk read!", ENDL, 0
msg_disk_success: db "Disk read successully!!", ENDL, 0

KERNEL_OFFSET   equ 0x1000    ; where to load the kernel
KERNEL_OFF      equ 0x00

kbdData         equ 0x60
kbdCommand      equ 0x64
kbdDisable      equ 0xAD
kbdEnable       equ 0xAE
kbdRead         equ 0xD0
kbdWrite        equ 0xD1

; padd with zeroes
times 510 - ($ - $$) db 0

; magic byte
dw 0xaa55
