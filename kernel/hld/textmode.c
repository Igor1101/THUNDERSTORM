/* THUNDERSTORM VGA HL DRIVER */
#include <stdint.h>
#include <TH/lld.h>
static volatile 
struct Text_mode_pointer vga =
{
  1, 1, Green
};
void tui_init(int color)
{
  vga.row = 1;
  vga.col = 1;
  vga.color=color;
}
void newline(void)
{
  if(vga.row >=ROWS)
  {
    make_newline();
    vga.col = 1;
    return;
  }
  vga.row++;
  vga.col = 1;
}
void kputchar(int8_t chr)
{
  switch(chr)
  {
    case '\n': newline(); return;
    case '\r': vga.col=1; return;
    case '\b': vga.col--; return;
    default: break;
  }
  kputchar_to(chr, vga.row, vga.col, vga.color);
  if(vga.col >= COLUMNS && vga.row < ROWS)
  {
    newline();
  }
  else if(vga.row >= ROWS && vga.col >= COLUMNS)
  {
    make_newline();
    vga.col = 1;
    vga.row = ROWS-1;
  }
  else
  {
    vga.col++;
  }
}
void kputs(char* str)
{
#ifdef KDEBUG
  kpause();
#endif
  while(*str!=0)
  {
    kputchar(*str);
    str++;
  }
  kputchar('\n');
}
