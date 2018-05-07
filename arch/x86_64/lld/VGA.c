/* THUNDERSTORM VGA LL DRIVER */
#ifdef USE_VGA
#include <stdint.h>
#include <TH/lld.h>
#include <string.h>
#include <cpu_management.h>
#define VGAADDR (volatile void* volatile)0xb8000

void update_cursor(int x, int y)
{
	uint16_t offset = (y * COLUMNS) + x;
	outb(0x3D4, 14);
  outb(0x3D5, offset >> 8);
  outb(0x3D4, 15);
  outb(0x3D5, offset);
}
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3E0) & 0xE0) | cursor_end);
}

void kputchar_to(int8_t chr, uint8_t row, uint8_t col, int color)
{
  register volatile int16_t *vga_addr \
    = VGAADDR + 2 * (row * 80 + col);
  *vga_addr = (uint16_t)chr | ((uint16_t)color)<<8;
}

void make_newline(void)
{
  memcpy((void*)VGAADDR, (void*)VGAADDR + 160, ROWS*COLUMNS*2 + COLUMNS * 2);
}

#endif
