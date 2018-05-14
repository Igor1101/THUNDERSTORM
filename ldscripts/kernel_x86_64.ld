ENTRY(_start)

	offset = 0x00000000;
SECTIONS {
	. = 1M;
	kernel_phys_base = . - offset;
	kernel_virt_base = .;

	.text BLOCK(4K) : AT(ADDR(.text) - offset) ALIGN(4K) {
    kernel_text = .;
		*(.multiboot)
		*(.text)
		*(.rodata)
	}
	
	.data BLOCK(4K) : AT(ADDR(.data) - offset) ALIGN(4K) {
    kernel_data = .;
		*(.data)
	}
	
	.bss BLOCK(4K) : AT(ADDR(.bss) - offset) ALIGN(4K) {
    kernel_bss = .;
		*(.bss)
    *(.noinit)
		*(COMMON)
	}
	
}
