/* THUNDERSTORM VGA LL DRIVER */
#ifdef USE_VGA
#include <stdint.h>
#include <TH/lld.h>
#include <string.h>
#define VGAADDR (void*)0xb8000

void kputchar_to(int8_t chr, uint8_t row, uint8_t col, int color)
{
  register volatile int16_t *vga_addr \
    = VGAADDR + 2 * (row * 80 + col);
  *vga_addr = (uint16_t)chr | ((uint16_t)color)<<8;
}

void make_newline(void)
{
  memcpy(VGAADDR, VGAADDR + 2 * ( 1 * 80 + 0 ), ROWS*COLUMNS*2);
}

#endif
