#Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>

BIOS ?=biosfile
QEMU_MEM ?= 100M
ARCH ?= x86_64
include compile_opt_kernel.mk
include parse_options.mk
INCLUDE_DIRS = -I usr/include\
	       -I arch/$(ARCH)/include\
	       -I kernel/acpi/source/include
KERNEL = boot/THkernel
TH_ABS_PATH = $(PWD)
CC ?= gcc
AS = as# now we aren`t really use it
LD = ld
AR = ar
OBJCOPY = objcopy
AS_FLAGS =
CC_FLAGS = -O3 -g -fno-stack-protector\
 	-ffreestanding -Wall  -Wextra -static -nostdlib 
KERNEL_CC_FLAGS = -mgeneral-regs-only -nostdinc -mno-red-zone $(CC_FLAGS) $(KERNEL_OPTIONS) $(INCLUDE_DIRS)
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
CDROMIMAGE = os.iso
DIRECTORIES:=src arch bin boot kernel libc \
	$(KERNEL_BUILD_PATH)\
	$(LIBC_BUILD_PATH)
all: kernel iso ksize 
kernel: initialize ports libc 
	# KERNEL COMPILATION
	@for cfile in $(KERNEL_C_SOURCES); do \
		CFILE=$(KERNEL_BUILD_PATH)/$$(basename $$cfile.o); \
		if [ ! -f $$CFILE ] || [ $$cfile -nt $$CFILE ] ;\
		then \
			printf 'COMPILING:\033[32m %s -> %s\n\033[0m', $$cfile, $$CFILE ; \
			$(CC) -c $$cfile -o $$CFILE $(KERNEL_CC_FLAGS) ;\
		fi;\
	done
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
		if [ ! -f $$CFILE ] || [ $$cfile -nt $$CFILE ] ;\
		then \
			printf 'COMPILING:\033[32m %s -> %s\n\033[0m', $$cfile, $$CFILE ;\
			nasm $(KERNEL_OPTIONS) -f elf64  -o $$CFILE $$cfile; \
		fi;\
	done
	# C PORTS
	@for cfile in $(LLD_C_SOURCES); do \
		CFILE=$(KERNEL_BUILD_PATH)/$$(basename $$cfile.o); \
		if [ ! -f $$CFILE ] || [ $$cfile -nt $$CFILE ] ;\
		then \
			printf 'COMPILING:\033[32m %s -> %s\n\033[0m', $$cfile, $$CFILE ; \
			$(CC) -c $$cfile -o $$CFILE $(KERNEL_CC_FLAGS) ; \
		fi;\
	done

# compile libraries:
libc: initialize
	# LIBC
	@for cfile in $(LIBC_C_SOURCES); do \
		CFILE=$(LIBC_BUILD_PATH)/$$(basename $$cfile.o); \
		if [ ! -f $$CFILE ] || [ $$cfile -nt $$CFILE ] ;\
		then \
			printf 'COMPILING:\033[32m %s -> %s\n\033[0m', $$cfile, $$CFILE ; \
			$(CC) -c $$cfile -o $$CFILE $(KERNEL_CC_FLAGS); \
		fi;\
	done; 
	# LIBC COMPRESSING
	$(AR) $(ARFLAGS) $(THLIBC) $(LIBC_BUILD_PATH)/*

iso: kernel
	@if [ -e  $(CDROMIMAGE) ]; then rm $(CDROMIMAGE) ; fi
	@if [ -e  /tmp/.THUNDERSTORM ]; then rm -r /tmp/.THUNDERSTORM ; fi
	if [ -f /usr/bin/grub2-mkrescue ] ; \
		then MKISOIMAGE=/usr/bin/grub2-mkrescue;\
		else MKISOIMAGE=/usr/bin/grub-mkrescue;\
	fi;\
	mkdir /tmp/.THUNDERSTORM;\
	cp -r ./boot /tmp/.THUNDERSTORM;\
	$$MKISOIMAGE  \
		-o $(CDROMIMAGE) /tmp/.THUNDERSTORM 2> /dev/null
	rm -r /tmp/.THUNDERSTORM
run_qemu: $(CDROMIMAGE)
	@if [ -f $(BIOS) ]; then \
	qemu-system-x86_64 -bios $(BIOS) -m $(QEMU_MEM) -cdrom $(CDROMIMAGE); \
	else \
	qemu-system-x86_64 -m $(QEMU_MEM) -cdrom $(CDROMIMAGE);fi
run: $(CDROMIMAGE)
	@if [ -f $(BIOS) ]; then \
	qemu-system-x86_64 -serial mon:stdio -display none\
		-bios $(BIOS) -m $(QEMU_MEM) -cdrom $(CDROMIMAGE); \
	else \
	qemu-system-x86_64 -serial mon:stdio -display none \
	-m $(QEMU_MEM) -cdrom $(CDROMIMAGE);fi
run_bochs: $(CDROMIMAGE)
	@bochs -q
debug: $(CDROMIMAGE)
	@if [ -f $(BIOS) ]; then \
	qemu-system-x86_64 -smp 10 -serial mon:stdio \
	-S -s -d int -bios $(BIOS) -m $(QEMU_MEM) -cdrom $(CDROMIMAGE);\
	else \
	qemu-system-x86_64 -smp 10 -serial mon:stdio \
	-S -s -d int -m $(QEMU_MEM) -cdrom $(CDROMIMAGE);fi
initialize: # add directories
	for dir in $(DIRECTORIES); do \
		if [ ! -e $$dir ]; then mkdir $$dir; fi;\
 	done
ksize: kernel
	@size  $(KERNEL)
	@printf "ISO size: "
	@du -h $(CDROMIMAGE) 
dis: kernel
	@objdump --disassembler-options=intel -D $(KERNEL) | less
nm: kernel
	@nm --numeric-sort $(KERNEL)|less
help:
	@printf ' make <\033[32mtarget\033[0m> \n'
	@printf ' note, that if you modified any *.c, *.asm, *.S file, it `ll be recognized and\n'
	@printf ' recompiled automatically, if there are another changes, please run `make clean` before recompilation\n'
	@printf '	targets: \n'
	@printf ' \033[32mnm\033[0m: print kernel symbols (less)\n'
	@printf ' \033[32mdis\033[0m: print intell syntax dissassembly (less)\n'
	@printf ' \033[32minitialize\033[0m: dont use this! just makes sure all source \
	directories available \n'
	@printf ' \033[32mdebug\033[0m: open qemu in waiting for gdb state \n'
	@printf ' \033[32mrun\033[0m : open qemu serial in console, don`t use graphics \n'
	@printf '	\033[32mrun_qemu\033[0m: run qemu with graphics \n'
	@printf ' \033[32mrun_bochs\033[0m: run under BOCHS emulator instead of qemu\n'
	@printf '	\033[32miso\033[0m: build iso image with kernel inside\n'
	@printf ' \033[32mlibc\033[0m: build kernel libraries\n'
	@printf ' \033[32mclean\033[0m: clean all the staff we built\n'
	@printf ' \033[32mkernel\033[0m: build kernel, put it to boot/ \n'
	@printf ' \033[32mports\033[0m: build $$(ARCH) ports of kernel\n'
	@printf ' \033[32mall\033[0m: will handle kernel, ports, libc.. \
	all you need to run os after\n'

