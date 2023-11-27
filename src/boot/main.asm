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
    ;cll
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


    jmp KERNEL_OFFSET

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
    push ax
    push bx
    push cx
    push dx
    push di
    push si
    push bp

.disk_load:
    mov di, KERNEL_OFFSET
    mov si, KERNEL_LBA

.read_sectors_loop:
    mov bp, 3                   ; num of retries 
    call lba_to_chs             ; convert lba to chs
    mov bx, di
    ;xor bx, bx                  ; set where to load the kernel

.retry:
    mov ax, 0x0201               ; ah = mode, 0x02 - read sectors from disk | al = num of sectors to read

    int 0x13                    ; call the interrupt
    jc .disk_error              ; if carry flag set -> error

.success:
    add di, 512 
    inc si

.chk_for_last_sector:
    cmp si, NUM_OF_SECTORS
    jl .read_sectors_loop

.done:
    pop bp
    pop si
    pop di
    pop dx
    pop cx
    pop bx
    pop ax
    ret

.disk_error:
    xor ah, ah
    int 13h
    dec bp
    test bp, bp
    jnz .retry

.error_end:
    mov si, msg_disk_error
    call puts
    call wait_key_and_reboot

;
; LBA to CHS
;   Sector      = (LBA % SPT) + 1      SPT = SectorsPerTrack
;   Head        = (LBA / SPT) % Heads
;   Cylinder    = (LBA / SPT) / Heads
;
; Inputs:   si = LBA
; Outputs:  dl = boot drive
;           dh = head
;           ch = cylinder (lower 8 bit of cylinder)
;           cl = upper 2 bit is upper 2 bit of cylinder and the rest is sector
;           

lba_to_chs:
    push ax 
    mov ax, si
    xor dx, dx          ; clear dx
    div word [SPT]      ; ax = LBA / SPT | dx = LBA % SPT
    mov cl, dl
    inc cl              ; cl = (LBA % SPT) + 1
    
    xor dx, dx
    div word [numHeads] ; ax = (LBA / SPT) / HEADS | dx = (LBA / SPT) % HEADS
    mov dh, dl          ; dh = (LBA / SPT) % HEADS 
    mov dl, [BOOT_DRIVE]
    mov ch, al          ; ch = lower 8 bit of - (LBA / SPT) / HEADS
    shl ah, 6           ; store only lower 2 bit of ah
    or cl, ah           ; upper 2 bit of sector in upper 2 bit
    pop ax
    ret
    
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
    cli
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

BOOT_DRIVE  db 0                ; boot drive variable
numHeads    dw 2                ; floppy disk has 2 heads and 18 sectors per track
SPT         dw 18

msg_hello: db "hello there!", ENDL, 0
msg_disk_error: db "Error in disk read!", ENDL, 0
msg_disk_success: db "Disk read successully!!", ENDL, 0

NUM_OF_SECTORS  equ 0x36        ; number of sectors to read from the disk

KERNEL_LBA      equ 1
KERNEL_OFFSET   equ 0x1000      ; where to load the kernel
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
