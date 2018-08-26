/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <stdint.h>
#include <kstdio.h>
#include <TH/kernel.h>
#include <TH/lld.h>
#include <TH/sysvars.h>

/*declared in kstdlib.h */
void show_progress(void)
{
        text.select_fgcolor(DefaultLogFG);
        text.putchar('\b');
        static uint8_t stat;
        switch (stat) {
        case 0:
                text.putchar('\\');
                stat++;
                break;
        case 1:
                text.putchar('|');
                stat++;
                break;
        case 2:
                text.putchar('/');
                stat++;
                break;
        case 3:
                text.putchar('-');
                stat = 0;
                break;
        default:
                text.putchar('+');
                stat = 0;
        }
        text.select_fgcolor(DefaultFG);
}

void init_progress(void)
{
        pr_cont("\t\t");
}

void stop_progress(void)
{
        pr_cont("\t\t");
}
