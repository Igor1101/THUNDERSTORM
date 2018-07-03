/* 
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com> 
 * THUNDERSTORM VGA HL DRIVER *
 */
#include <stdint.h>
#include <TH/lld.h>
#include <asm/serial.h>
#define BEGINNING 0
volatile 
struct Text_mode_pointer text;
void tui_init(void)
{
  text.row = BEGINNING;
  text.col = BEGINNING;
  text.fgcolor = Default;
  enable_cursor(BEGINNING, text.rows );
  text.columns = determine_columns();
  text.rows = determine_rows();
#ifdef USE_SERIAL
  /*
  serial_disable_ints(SERIAL_MAIN);
  serial_configure_fifo(SERIAL_MAIN);
  serial_configure_line(SERIAL_MAIN);
  serial_configure_baud_rate(SERIAL_MAIN, 3);// 38400 
  */
#endif /* USE_SERIAL */
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
  if(text.row >= text.rows - 1)
  {
    make_newline();
    text.col = BEGINNING;
    return;
  }
  text.row++;
  text.col = BEGINNING;
}
void kputchar(int8_t chr)
{
#ifdef USE_SERIAL
  serial_write_asyn(SERIAL_MAIN, chr);
  if(chr=='\n')
    serial_write_asyn(SERIAL_MAIN, '\r');
#endif
  switch(chr)
  {
    case '\n': 
                /* clear last cursor */
               kputchar_to(' ', text.row, text.col, Black, Black, NOTRANSPARENT);
               newline();
               update_cursor(text.row, text.col + 1);
               return;
    case '\r': 
                /* clear last cursor */
               kputchar_to(' ', text.row, text.col, Black, Black, NOTRANSPARENT);
               text.col = BEGINNING; 
               update_cursor(text.row, text.col + 1);
               return;
    case '\b': 
               text.col--; 
               update_cursor(text.row, text.col + 1);
               return;
    default: 
               break;
  }
  update_cursor(text.row, text.col + 1);
  kputchar_to(chr, text.row, text.col, 
      text.fgcolor, text.bgcolor, NOTRANSPARENT);
  if(text.col >= text.columns && text.row < text.rows)
  {
    newline();
  }
  else if(text.row >= text.rows && text.col >= text.columns)
  {
    make_newline();
    text.col = BEGINNING;
    text.row = text.rows-1;
  }
  else
  {
    text.col++;
  }
}
void kputs(char * str)
{
  while(*str != 0)
  {
    kputchar(*str);
    str++;
  }
  kputchar('\n');
}
