ASM=nasm
CC=/home/neta/opt/cross/bin/i386-elf-gcc
LD=/home/neta/opt/cross/i386-elf/bin/ld
RM=rm
OBJCOPY=objcopy

SRC=src
BUILD=build


$(BUILD)/os-image.img: boot kernel
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$(BUILD)/boot.bin of=$@ conv=notrunc bs=512 count=1 seek=0
	dd if=$(BUILD)/kernel.bin of=$@ conv=notrunc bs=512 count=2048 seek=1


boot: $(BUILD)/boot.bin 

$(BUILD)/boot.bin: always
	$(MAKE) -C $(SRC)/boot BUILD=$(abspath $(BUILD))


kernel: $(BUILD)/kernel.bin

$(BUILD)/kernel.bin: always
	$(MAKE) -C $(SRC)/kernel BUILD=$(abspath $(BUILD))


always: 
	mkdir -p $(BUILD)

run: $(BUILD)/os-image.img
	# qemu-system-i386 -serial file:serial.log -fda $< 
	qemu-system-i386 -serial stdio -fda $< 

debug: $(BUILD)/os-image.img
	bochs-debugger -f bochs_config
	# for logs -- log: bochslog.txt

clean:
	@$(MAKE) -C $(SRC)/boot BUILD=$(abspath $(BUILD)) clean
	@$(MAKE) -C $(SRC)/kernel BUILD=$(abspath $(BUILD)) clean

