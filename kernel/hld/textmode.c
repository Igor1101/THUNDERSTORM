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
  vga.row = 0;
  vga.col = 0;
  vga.color=color;
  enable_cursor(0, 255);
}
void newline(void)
{
  if(vga.row >=ROWS)
  {
    make_newline();
    vga.col = 0;
    return;
  }
  vga.row++;
  vga.col = 0;
}
void kputchar(int8_t chr)
{
  switch(chr)
  {
    case '\n': newline();
               update_cursor(vga.col + 1, vga.row);
               return;
    case '\r': vga.col=0; 
               update_cursor(vga.col + 1, vga.row);
               return;
    case '\b': vga.col--; 
               update_cursor(vga.col + 1, vga.row);
               return;
    default: break;
  }
  kputchar_to(chr, vga.row, vga.col, vga.color);
  update_cursor(vga.col + 1, vga.row);
  if(vga.col >= COLUMNS && vga.row < ROWS)
  {
    newline();
  }
  else if(vga.row >= ROWS && vga.col >= COLUMNS)
  {
    make_newline();
    vga.col = 0;
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
