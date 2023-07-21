BUILD?=build/
ASM?=nasm
ASMFLAGS?=-f elf
CC?=/home/neta/opt/cross/bin/i386-elf-gcc
LD=/home/neta/opt/cross/bin/i386-elf-ld
LGCC=/home/neta/opt/cross/lib/gcc/i386-elf/13.1.0
# CCFLAGS= -ffreestanding -nostdlib -c
CCFLAGS=-m32 -std=c11 -O2 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CCFLAGS+=-Wno-pointer-arith -Wno-unused-parameter
CCFLAGS+=-nostdlib -nostdinc -ffreestanding -fno-pie -fno-stack-protector
CCFLAGS+=-fno-builtin-function -fno-builtin -c -mgeneral-regs-only
LDFLAGS=-ffreestanding -O2 -nostdlib -m32 -L $(LGCC) -lgcc

SOURCES_C=$(wildcard *.c)
SOURCES_ASM=$(wildcard *.asm)
OBJECTS_C=$(patsubst %.c,$(BUILD)/kernel/c/%.obj,$(SOURCES_C))
OBJECTS_ASM=$(patsubst %.asm,$(BUILD)/kernel/asm/%.obj,$(SOURCES_ASM))

.PHONY: all clean always

all: kernel.bin

kernel.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	$(CC) -T link.ld -o $(BUILD)/kernel.bin $^ $(LDFLAGS)
	# $(LD) -o $(BUILD)/kernel.bin $(LDFLAGS) -Tlink.ld $^

$(BUILD)/kernel/c/%.obj: %.c always
	$(CC) $(CCFLAGS) -o $@ $<

$(BUILD)/kernel/asm/%.obj: %.asm always
	$(ASM) $(ASMFLAGS) -o $@ $<

always:
	mkdir -p $(BUILD)/kernel/c
	mkdir -p $(BUILD)/kernel/asm

clean:
	rm -f $(BUILD)/kernel.bin
