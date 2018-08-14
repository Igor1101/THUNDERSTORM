/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <stdint.h>
#include <kstdio.h>
#include <TH/kernel.h>
#include <TH/lld.h>

/*declared in kstdlib.h */
void show_progress(void)
{
        select_fgcolor(DefaultLogFG);
        kputchar('\b');
        static uint8_t stat;
        switch (stat) {
        case 0:
                kputchar('\\');
                stat++;
                break;
        case 1:
                kputchar('|');
                stat++;
                break;
        case 2:
                kputchar('/');
                stat++;
                break;
        case 3:
                kputchar('-');
                stat = 0;
                break;
        default:
                kputchar('+');
                stat = 0;
        }
        select_fgcolor(DefaultFG);
}

void init_progress(void)
{
        pr_cont("\t\t");
}

void stop_progress(void)
{
        pr_cont("\t\t");
}
