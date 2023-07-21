BUILD?=build/
ASM?=nasm

.PHONY: all clean

all: $(BUILD)/boot.bin

$(BUILD)/boot.bin: main.asm
	$(ASM) main.asm -f bin -o $(BUILD)/boot.bin

clean: 
	rm -f $(BUILD)/boot.bin

