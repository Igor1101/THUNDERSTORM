/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <stdint.h>
#include <kstdio.h>

/*declared in kstdlib.h */
void show_progress(void)
{
  static uint8_t stat;
  switch(stat)
  {
    case 0: kputchar('\\');
            stat++;
            break;
    case 1: kputchar('|');
            stat++;
            break;
    case 2: kputchar('/');
            stat++;
            break;
    case 3: kputchar('-');
            stat = 0;
            break;
    default:
            kputchar('+'); 
            stat = 0;
  }
  kputchar('\b');
}

void init_progress(void)
{
  kprintf("\n <---------\r");
}
void stop_progress(void)
{
  kputchar('\n');
}
