ENTRY(_start)

	offset = 0x00000000;
SECTIONS {
	. = 1M;
	kernel_phys_base = . - offset;
	kernel_virt_base = .;

	.text BLOCK(4K) : AT(ADDR(.text) - offset) ALIGN(4K) {
		*(.multiboot)
		*(.text)
		*(.rodata)
	}
	
	.data BLOCK(4K) : AT(ADDR(.data) - offset) ALIGN(4K) {
		*(.data)
	}
	
	.bss BLOCK(4K) : AT(ADDR(.bss) - offset) ALIGN(4K) {
		*(.bss)
    *(.noinit)
		*(COMMON)
	}
	
}
