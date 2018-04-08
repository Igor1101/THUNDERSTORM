.PHONY: kernel
.PHONY: ksize
.PHONY: libc
ARCH ?= x86_64
kernel=TH
KERNEL_ABS_PATH=$(PWD)
CC=gcc
AS=as# now we aren`t really use it
LD=ld
AS_FLAGS:=
CC_FLAGS:= -O0 -g -ffreestanding -Wall -Wextra -static -nostdlib -I include 
LD_FLAGS:=-nostdlib -static 

BOOT_PORTS_PATH:=arch/$(ARCH)/boot
BOOT_PORTS += boot multiboot print kernel_init
LLD_PORTS_PATH:=arch/$(ARCH)/lld
# high level drivers
HLD += textmode
HLD_PATH=kernel/hld
LLD_PORTS += VGA
KERN_SOURCES = main
KERNEL_OBJECTS:=

LIBC_DIRECTORIES=libc libc/string

DIRECTORIES:=src arch bin boot include build kernel libc
all: initialize kernel iso ksize
kernel: ports hld libc
	for cfile in $(KERN_SOURCES); do \
		$(CC) -c kernel/$$cfile.c -o build/$$cfile.o $(CC_FLAGS) \
	; done
	$(LD) --nmagic -o boot/$(kernel).elf -T ldscripts/kernel_x86_64.ld build/*.o $(LD_FLAGS) 

clean:
	rm -r build/*.o
	rm os.iso
ports:
	for file in $(BOOT_PORTS);do \
	nasm -f elf64  -o build/$$file.o $(BOOT_PORTS_PATH)/$$file.asm\
	;done
	for cfile in $(LLD_PORTS); do \
		$(CC) -c $(LLD_PORTS_PATH)/$$cfile.c -o build/$$cfile.o $(CC_FLAGS) \
	; done
hld: ports
	for cfile in $(HLD); do \
		$(CC) -c $(HLD_PATH)/$$cfile.c -o build/$$cfile.o $(CC_FLAGS) \
	; done

# compile libraries:
libc:
	for libdir in $(LIBC_DIRECTORIES);do \
		cd $$libdir ;\
		for cfile in *.c; do \
			$(CC) -c $$cfile -o $(KERNEL_ABS_PATH)/build/$$cfile.o $(CC_FLAGS) \
		; done ;\
		cd - \
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
	@printf "ISO size: "
	@du -h os.iso
dis: kernel
	@objdump -D boot/$(kernel).elf | less
nm: kernel
	@nm --numeric-sort boot/$(kernel).elf|less


