#include <stdint.h>
#ifndef LLD_TH
#define LLD_TH
/*VGA info */
#define ROWS 25
#define COLUMNS 80
enum Color
{
  Black      = 0,
  Blue       = 1,
  Green      = 2,
  Cyan       = 3,
  Red        = 4,
  Magenta    = 5,
  Brown      = 6,
  LightGray  = 7,
  DarkGray   = 8,
  LightBlue  = 9,
  LightGreen = 10,
  LightCyan  = 11,
  LightRed   = 12,
  Pink       = 13,
  Yellow     = 14,
  White      = 15
};

struct Text_mode_pointer
{
  uint8_t volatile row;
  uint8_t volatile col;
  int volatile color;
};
void vga_init(int color);
void kputchar_to(int8_t chr, uint8_t row, uint8_t col, int color);
void kputchar(int8_t chr);
#endif