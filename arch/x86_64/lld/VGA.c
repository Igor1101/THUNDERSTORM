/* THUNDERSTORM VGA LL DRIVER */
#include <stdint.h>
#include <TH/lld.h>
#define VGAADDR (void*)0xb8000

void kputchar_to(int8_t chr, uint8_t row, uint8_t col, int color)
{
  register volatile int16_t *vga_addr \
    = VGAADDR + 2 * (row * 80 + col);
  *vga_addr = (uint16_t)chr | ((uint16_t)color)<<8;
}
