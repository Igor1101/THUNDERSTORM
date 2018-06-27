include compile_opt_kernel.mk
BIOS ?=biosfile
QEMU_MEM ?= 100M
KERNEL_OPTIONS += -D $(KERNEL_OUTPUT) -D $(KERNEL_BUILD_MODE)
ARCH ?= x86_64
INCLUDE_DIRS = -I usr/include -I arch/$(ARCH)/include
KERNEL = boot/THkernel
TH_ABS_PATH = $(PWD)
CC ?= gcc
AS = as# now we aren`t really use it
LD = ld
AR = ar
OBJCOPY = objcopy
AS_FLAGS =
CC_FLAGS = -Og -g -fno-stack-protector\
<<<<<<< HEAD
 	-ffreestanding -Wall  -Werror -Wextra -static -nostdlib -Wno-unused-parameter
KERNEL_CC_FLAGS = -mgeneral-regs-only -nostdinc $(CC_FLAGS) $(KERNEL_OPTIONS) $(INCLUDE_DIRS)
=======
 	-ffreestanding -Wall  -Werror -Wextra -static -nostdlib  $(KERNEL_OPTIONS) $(INCLUDE_DIRS)
>>>>>>> fbc8d093eb777ef79fbe6b6d86d5d2412ae235f2
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
<<<<<<< HEAD
CDROMIMAGE = os.iso
=======
CDROMIMAGE = cdromimage/os.iso
>>>>>>> fbc8d093eb777ef79fbe6b6d86d5d2412ae235f2

DIRECTORIES:=src arch bin boot kernel libc \
	$(KERNEL_BUILD_PATH)\
	$(LIBC_BUILD_PATH)
all: kernel iso ksize 
kernel: initialize ports libc 
	# KERNEL COMPILATION
	@for cfile in $(KERNEL_C_SOURCES); do \
		CFILE=$(KERNEL_BUILD_PATH)/$$(basename $$cfile.o); \
		printf 'COMPILING:\033[32m %s -> %s\n\033[0m', $$cfile, $$CFILE ; \
<<<<<<< HEAD
		$(CC) -c $$cfile -o $$CFILE $(KERNEL_CC_FLAGS) \
=======
		$(CC) -c $$cfile -o $$CFILE $(CC_FLAGS) \
>>>>>>> fbc8d093eb777ef79fbe6b6d86d5d2412ae235f2
	; done
	$(LD) --nmagic -o $(KERNEL) -T ldscripts/kernel_x86_64.ld $(KERNEL_BUILD_PATH)/* $(LD_FLAGS) $(THLIBC)

clean:
	rm -r $(KERNEL_BUILD_PATH)
	rm -r $(LIBC_BUILD_PATH)
	rm -r $(THLIBC)
	rm $(CDROMIMAGE) 
	rm -r $(KERNEL)
ports: initialize
	# NASM PORTS
	@for cfile in $(LLD_NASM_SOURCES);do \
	CFILE=$(KERNEL_BUILD_PATH)/$$(basename $$cfile.o); \
	printf 'COMPILING:\033[32m %s -> %s\n\033[0m', $$cfile, $$CFILE ; \
	nasm $(KERNEL_OPTIONS) -f elf64  -o $$CFILE $$cfile; \
	done
	# C PORTS
	@for cfile in $(LLD_C_SOURCES); do \
		CFILE=$(KERNEL_BUILD_PATH)/$$(basename $$cfile.o); \
		printf 'COMPILING:\033[32m %s -> %s\n\033[0m', $$cfile, $$CFILE ; \
<<<<<<< HEAD
		$(CC) -c $$cfile -o $$CFILE $(KERNEL_CC_FLAGS) \
=======
		$(CC) -c $$cfile -o $$CFILE $(CC_FLAGS) \
>>>>>>> fbc8d093eb777ef79fbe6b6d86d5d2412ae235f2
	; done

# compile libraries:
libc: initialize
	# LIBC
	@for cfile in $(LIBC_C_SOURCES); do \
		CFILE=$(LIBC_BUILD_PATH)/$$(basename $$cfile.o); \
		printf 'COMPILING:\033[32m %s -> %s\n\033[0m', $$cfile, $$CFILE ; \
<<<<<<< HEAD
		$(CC) -c $$cfile -o $$CFILE $(KERNEL_CC_FLAGS); \
=======
		$(CC) -c $$cfile -o $$CFILE $(CC_FLAGS); \
>>>>>>> fbc8d093eb777ef79fbe6b6d86d5d2412ae235f2
	done; 
	# LIBC COMPRESSING
	$(AR) $(ARFLAGS) $(THLIBC) $(LIBC_BUILD_PATH)/*

iso: kernel
	if [ -e  $(CDROMIMAGE) ]; then rm $(CDROMIMAGE) ; fi
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
	@size  $(KERNEL)
	@printf "ISO size: "
	@du -h $(CDROMIMAGE) 
dis: kernel
	@objdump --disassembler-options=intel -D $(KERNEL) | less
nm: kernel
	@nm --numeric-sort $(KERNEL)|less

