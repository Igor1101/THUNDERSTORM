.PHONY: kernel
.PHONY: ksize
.PHONY: libc
ARCH ?= x86_64
kernel=TH
TH_ABS_PATH=$(PWD)
CC ?= gcc
AS = as# now we aren`t really use it
LD = ld
AR = ar
AS_FLAGS:=
CC_FLAGS:= -Os -g -ffreestanding -Wall -Wextra -static -nostdlib -I include 
LD_FLAGS:=-nostdlib -static 
BOOT_PORTS_PATH:=arch/$(ARCH)/boot
BOOT_PORTS += boot multiboot print kernel_init
LLD_PORTS_PATH:=arch/$(ARCH)
# high level drivers
LLD_PORTS += VGA
LLD_NASM_SOURCES=$(shell find $(LLD_PORTS_PATH) -name *.asm)
LLD_AS_SOURCES=$(shell find $(LLD_PORTS_PATH) -name *.s)
LLD_AS_SOURCES += $(shell find $(LLD_PORTS_PATH) -name *.S)
LLD_C_SOURCES = $(shell find $(LLD_PORTS_PATH) -name *.c)
KERNEL_C_SOURCES=$(shell find kernel/ -name *.c)
KERNEL_BUILD_PATH=kernbuild# where to put all kernel *.o files
THLIBC=libTHc.a 
LIBC_C_SOURCES=$(shell find libc/ -name *.c)
LIBC_BUILD_PATH=libcbuild
LIBC_DIRECTORIES=libc libc/string

DIRECTORIES:=src arch bin boot include kernel libc \
	$(KERNEL_BUILD_PATH)\
	$(LIBC_BUILD_PATH)
all: initialize kernel iso ksize
kernel: ports libc
	for cfile in $(KERNEL_C_SOURCES); do \
		CFILE=$(KERNEL_BUILD_PATH)/$$(basename $$cfile.o); \
		printf 'COMPILING:\033[32m %s -> %s\n\033[0m', $$cfile, $$CFILE ; \
		$(CC) -c $$cfile -o $$CFILE $(CC_FLAGS) \
	; done
	$(LD) --nmagic -o boot/$(kernel).elf -T ldscripts/kernel_x86_64.ld $(KERNEL_BUILD_PATH)/* $(LD_FLAGS) $(THLIBC)

clean:
	rm -r $(KERNEL_BUILD_PATH)
	rm -r $(LIBC_BUILD_PATH)
	rm -r $(THLIBC)
	rm os.iso
ports:
	# compile NASM syntax ports sorces
	for cfile in $(LLD_NASM_SOURCES);do \
	CFILE=$(KERNEL_BUILD_PATH)/$$(basename $$cfile.o); \
	printf 'COMPILING:\033[32m %s -> %s\n\033[0m', $$cfile, $$CFILE ; \
	nasm -f elf64  -o $$CFILE $$cfile; \
	done
	for cfile in $(LLD_C_SOURCES); do \
		CFILE=$(KERNEL_BUILD_PATH)/$$(basename $$cfile.o); \
		printf 'COMPILING:\033[32m %s -> %s\n\033[0m', $$cfile, $$CFILE ; \
		$(CC) -c $$cfile -o $$CFILE $(CC_FLAGS) \
	; done

# compile libraries:
libc:
	for cfile in $(LIBC_C_SOURCES); do \
		CFILE=$(LIBC_BUILD_PATH)/$$(basename $$cfile.o); \
		printf 'COMPILING:\033[32m %s -> %s\n\033[0m', $$cfile, $$CFILE ; \
		$(CC) -c $$cfile -o $$CFILE $(CC_FLAGS) \
	; done
	$(AR) $(ARFLAGS) $(THLIBC) $(LIBC_BUILD_PATH)/*

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


