.PHONY: kernel
.PHONY: ksize
ARCH ?= x86_64
kernel=TH
CC=gcc
AS=as# now we aren`t really use it
LD=ld
AS_FLAGS:=
CC_FLAGS:=-ffreestanding -Ikernel -Wall -Wextra -Os 
LD_FLAGS:=-nostdlib

PORTS_PATH:=arch/$(ARCH)/boot
PORTS += boot multiboot print kernel_init

KERNEL_OBJECTS:=

DIRECTORIES:=src arch bin boot include build kernel
all: initialize kernel iso ksize
kernel: ports
	$(LD) --nmagic -o boot/$(kernel).elf -T ldscripts/kernel_x86_64.ld build/*.o

clean:
	rm -r build/*.o
	rm os.iso
ports:
	for file in $(PORTS);do \
	nasm -f elf64  -o build/$$file.o $(PORTS_PATH)/$$file.asm\
	;done
iso: kernel
	if [ -e os.iso ]; then rm os.iso ; fi
	grub-mkrescue -o os.iso ./ 2> /dev/null
run:
	@qemu-system-x86_64 -m 256M -cdrom os.iso
debug:
	@qemu-system-x86_64 -S -s -m 120M -cdrom os.iso 
initialize: # add directories
	for dir in $(DIRECTORIES); do \
	if ! [ -e $$dir ]; then mkdir $$dir; fi;\
 	done
ksize: kernel
	@size boot/$(kernel).elf
	@printf "cd size: "
	@du -h os.iso
dis: kernel
	@objdump -d boot/$(kernel).elf | less
