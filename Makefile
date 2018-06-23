.PHONY: kernel
.PHONY: ksize
.PHONY: libc
BIOS ?=biosfile
QEMU_MEM ?= 100M
KERNEL_OPTIONS ?=  -D USE_VESA
ARCH ?= x86_64
INCLUDE_DIRS = -I usr/include -I arch/$(ARCH)/include
kernel = TH
TH_ABS_PATH = $(PWD)
CC ?= gcc
AS = as# now we aren`t really use it
LD = ld
AR = ar
OBJCOPY = objcopy
AS_FLAGS =
CC_FLAGS = -O4 -g -fno-stack-protector\
 	-ffreestanding -Wall  -Werror -Wextra -static -nostdlib  $(KERNEL_OPTIONS) $(INCLUDE_DIRS)
LD_FLAGS = -nostdlib -static 
BOOT_PORTS_PATH = arch/$(ARCH)/boot
BOOT_PORTS += boot multiboot print kernel_init
LLD_PORTS_PATH = arch/$(ARCH)
KERNEL_BUILD_PATH = kernbuild# where to put all kernel *.o files
THLIBC = libTHc.a 
LIBC_BUILD_PATH = libcbuild
# high level drivers
# find more files we need:
LLD_NASM_SOURCES = $(shell find $(LLD_PORTS_PATH) -name *.asm)
LLD_AS_SOURCES = $(shell find $(LLD_PORTS_PATH) -name *.s)
LLD_AS_SOURCES += $(shell find $(LLD_PORTS_PATH) -name *.S)
LLD_C_SOURCES = $(shell find $(LLD_PORTS_PATH) -name *.c)
KERNEL_C_SOURCES = $(shell find kernel/ -name *.c)
LIBC_C_SOURCES = $(shell find libc/ -name *.c)
CDROMIMAGE = cdromimage/os.iso

DIRECTORIES:=src arch bin boot kernel libc \
	$(KERNEL_BUILD_PATH)\
	$(LIBC_BUILD_PATH)
all: kernel iso ksize 
kernel: initialize ports libc 
	# KERNEL COMPILATION
	@for cfile in $(KERNEL_C_SOURCES); do \
		CFILE=$(KERNEL_BUILD_PATH)/$$(basename $$cfile.o); \
		printf 'COMPILING:\033[32m %s -> %s\n\033[0m', $$cfile, $$CFILE ; \
		$(CC) -c $$cfile -o $$CFILE $(CC_FLAGS) \
	; done
	$(LD) --nmagic -o boot/$(kernel).elf -T ldscripts/kernel_x86_64.ld $(KERNEL_BUILD_PATH)/* $(LD_FLAGS) $(THLIBC)

clean:
	rm -r $(KERNEL_BUILD_PATH)
	rm -r $(LIBC_BUILD_PATH)
	rm -r $(THLIBC)
	rm $(CDROMIMAGE) 
ports: initialize
	# compile NASM syntax ports sorces
	@for cfile in $(LLD_NASM_SOURCES);do \
	CFILE=$(KERNEL_BUILD_PATH)/$$(basename $$cfile.o); \
	printf 'COMPILING:\033[32m %s -> %s\n\033[0m', $$cfile, $$CFILE ; \
	nasm $(KERNEL_OPTIONS) -f elf64  -o $$CFILE $$cfile; \
	done
	@for cfile in $(LLD_C_SOURCES); do \
		CFILE=$(KERNEL_BUILD_PATH)/$$(basename $$cfile.o); \
		printf 'COMPILING:\033[32m %s -> %s\n\033[0m', $$cfile, $$CFILE ; \
		$(CC) -c $$cfile -o $$CFILE $(CC_FLAGS) \
	; done

# compile libraries:
libc: initialize
	# LIBC BUILDING
	@for cfile in $(LIBC_C_SOURCES); do \
		CFILE=$(LIBC_BUILD_PATH)/$$(basename $$cfile.o); \
		printf 'COMPILING:\033[32m %s -> %s\n\033[0m', $$cfile, $$CFILE ; \
		$(CC) -c $$cfile -o $$CFILE $(CC_FLAGS); \
	done; \
	$(AR) $(ARFLAGS) $(THLIBC) $(LIBC_BUILD_PATH)/*

iso: kernel
	if [ -e os.iso ]; then rm $(CDROMIMAGE) ; fi
	grub-mkrescue  -o $(CDROMIMAGE) ./ 2> /dev/null
run: $(CDROMIMAGE)
	@if [ -f $(BIOS) ]; then \
	qemu-system-x86_64 -bios $(BIOS) -m $(QEMU_MEM) -cdrom $(CDROMIMAGE); \
	else \
	qemu-system-x86_64 -m $(QEMU_MEM) -cdrom $(CDROMIMAGE);fi
debug: $(CDROMIMAGE)
	@if [ -f $(BIOS) ]; then \
	qemu-system-x86_64 -S -s -d int -bios $(BIOS) -m $(QEMU_MEM) -cdrom $(CDROMIMAGE);\
	else \
	qemu-system-x86_64 -S -s -d int -m $(QEMU_MEM) -cdrom $(CDROMIMAGE);fi
initialize: # add directories
	for dir in $(DIRECTORIES); do \
		if ! [ -e $$dir ]; then mkdir $$dir; fi;\
 	done
ksize: kernel
	@size  boot/$(kernel).elf
	@printf "ISO size: "
	@du -h $(CDROMIMAGE) 
dis: kernel
	@objdump -D boot/$(kernel).elf | less
nm: kernel
	@nm --numeric-sort boot/$(kernel).elf|less

