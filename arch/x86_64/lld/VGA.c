/* THUNDERSTORM VGA LL DRIVER 
 * Note, that VGA 0xb8000 text support is provided 
 * for debbuging only purposes
 * */
#ifdef USE_VGA
#include <stdint.h>
#include <kstring.h>
#include <TH/lld.h>
#include <x86_64/cpu_management.h>

#define VGAADDR (volatile void* volatile)0xb8000
#define COLUMNS 80
#define ROWS 23

uint32_t determine_columns(void)
{
  /* these functions added to provide portability,
    encapsulation */
  return COLUMNS;
}
uint32_t determine_rows(void)
{
  /* these functions added to provide portability,
    encapsulation */
  return ROWS;
}
void update_cursor(int x, int y)
{
	uint16_t offset = (y * COLUMNS) + x + COLUMNS + 1;
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

void kputchar_to(
    /* unicode character */
    unsigned short int c,
    /* cursor position on screen in characters  */
    uint32_t cx, uint32_t cy,
    /* foreground and background colors */
    uint32_t fg, uint32_t bg,
    /* useless here */
    uint32_t attr
    )
{/*bgcolor - useless var */
  bg++;
  register volatile int16_t *vga_addr \
    = VGAADDR + 2 * (cx * 80 + cy);
  *vga_addr = (uint16_t)c | ((uint16_t)fg)<<8;
}

void make_newline(void)
{
#ifdef DEBUG
  kpause();
#endif
  memcpy((void*)VGAADDR, (void*)VGAADDR + 160, ROWS*COLUMNS*2 + COLUMNS * 2);
}

#endif
