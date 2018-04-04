.PHONY: kernel
ARCH ?= x86_64
kernel=TH
CC=gcc
AS=as# now we aren`t really use it
LD=ld
AS_FLAGS:=
CC_FLAGS:=-ffreestanding -Ikernel -Wall -Wextra -O2 
LD_FLAGS:=-nostdlib

KERNEL_OBJECTS:=
all: initialize kernel iso

clean:
	@rm -r bin/*
kernel:
	# compile ports:
	@nasm -g -f elf64 arch/$(ARCH)/boot/boot.asm -o build/boot.o
	@nasm -g -f elf64 arch/$(ARCH)/boot/multiboot.asm -o build/multiboot.o
	@$(LD) --nmagic -o boot/$(kernel).elf -T ldscripts/kernel_x86_64.ld build/*
iso: kernel
	@rm ./os.iso
	@grub-mkrescue -o os.iso ./
run:
	@qemu-system-x86_64 -m 12M -cdrom os.iso
debug:
	@qemu-system-x86_64 -S -s -m 12M -cdrom os.iso
initialize: # add directories
	@if ! [ -e src ]; then mkdir src;fi
	@if ! [ -e arch ]; then mkdir arch; fi
	@if ! [ -e bin ]; then mkdir bin; fi
	@if ! [ -e boot ]; then mkdir boot; fi
	@if ! [ -e build ]; then mkdir build; fi
	@if ! [ -e kernel ]; then mkdir kernel; fi
