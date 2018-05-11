/* THUNDERSTORM VGA HL DRIVER */
#include <stdint.h>
#include <TH/lld.h>
#define BEG 0
volatile 
struct Text_mode_pointer text;
void tui_init(void)
{
  text.row = BEG;
  text.col = BEG;
  text.fgcolor = Default;
  enable_cursor(BEG, text.rows + 2);
  text.columns = determine_columns();
  text.rows = determine_rows();
}

void select_fgcolor(int color)
{
  text.fgcolor = color;
}

void select_bgcolor(int color)
{
  text.bgcolor = color;
}

void newline(void)
{
  if(text.row >=text.rows)
  {
    make_newline();
    text.col = BEG;
    return;
  }
  text.row++;
  text.col = BEG;
}
void kputchar(int8_t chr)
{
  switch(chr)
  {
    case '\n': newline();
               update_cursor(text.row, text.col);
               return;
    case '\r': text.col=BEG; 
               update_cursor(text.row, text.col);
               return;
    case '\b': text.col--; 
               update_cursor(text.row, text.col);
               return;
    default: break;
  }
  kputchar_to(chr, text.row, text.col, text.fgcolor, text.bgcolor);
  update_cursor(text.col + 1, text.row);
  if(text.col >= text.columns && text.row < text.rows)
  {
    newline();
  }
  else if(text.row >= text.rows && text.col >= text.columns)
  {
    make_newline();
    text.col = BEG;
    text.row = text.rows-1;
  }
  else
  {
    text.col++;
  }
}
void kputs(char * str)
{
#ifdef KDEBUG
  kpause();
#endif
  while(*str != 0)
  {
    kputchar(*str);
    str++;
  }
  kputchar('\n');
}
