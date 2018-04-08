/* THUNDERSTORM VGA LL DRIVER */
#include <stdint.h>
#include <TH/lld.h>
static volatile 
struct Text_mode_pointer vga 
/*__attribute__((section(".rodata")))*/=
{
  1, 1, Green
};
void tui_init(int color)
{
  vga.row=1;
  vga.col=1;
  vga.color=color;
}
void newline(void)
{
  vga.row++;
  vga.col=1;
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
  if(vga.col >= COLUMNS && vga.row <= ROWS)
  {
    newline();
  }
  else if(vga.col >= COLUMNS && vga.row >= ROWS)
  {
    make_newline();
  }
  else
  {
    vga.col++;
  }
}
void kputs(int8_t * str)
{
  while(*str!=0)
  {
    kputchar(*str);
    str++;
  }
  kputchar('\n');
}
