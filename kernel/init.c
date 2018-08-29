/*
 _____ _   _ _   _ _   _ ____  _____ ____  ____ _____ ___  ____  __  __        __
|_   _| | | | | | | \ | |  _ \| ____|  _ \/ ___|_   _/ _ \|  _ \|  \/  |  __  /  \_  _
  | | | |_| | | | |  \| | | | |  _| | |_) \___ \ | || | | | |_) | |\/| |_/  \/     \/ \
  | | |  _  | |_| | |\  | |_| | |___|  _ < ___) || || |_| |  _ <| |  | |\_ ___________/
  |_| |_| |_|\___/|_| \_|____/|_____|_| \_\____/ |_| \___/|_| \_\_|  |_|   /
                                                                          /\_
                                                                          \  \
__________________________________________________________________________/___________
 
                       Embedded operating system
    Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

/* High level TH kernel initialization */

/* indentation from top of display in chars*/
#define OFFSET_FROM_TOP 8

#include <kstring.h>
#include <kstdlib.h>
#include <kstdio.h>
#include <stdbool.h>
#include <TH/lld.h>
#include <TH/sysinfo.h>
#include <TH/sysvars.h>
#include <TH/stack.h>
#include <linux/kernel.h>
#include <TH/kern_levels.h>
#include <TH/die.h>
#include <asm/int_handler.h>
#include <asm/cpu_management.h>
#include <asm/bootinfo.h>
#include <asm/serial.h>
#include <asm/kpanic.h>
/* additional ifaces: */
#include <acpi.h>

/* declared vars */
struct RAM_INFO RAM;
struct RAM_MAP ram_map[MAX_RAM_ENTRIES];

NORET VISIBLE int start_kernel(uintptr_t boot_magic, void *pcinfo)
{
#ifdef USE_SERIAL
        serial_disable_ints(SERIAL_MAIN);
        serial_configure_fifo(SERIAL_MAIN);
        serial_configure_line(SERIAL_MAIN);
#ifdef RELEASE
        /* baud rates setting faults on some x86_64 emulators */
        serial_configure_baud_rate(SERIAL_MAIN, 1);     // 115200 
#endif                          /* RELEASE */
#endif                          /* USE_SERIAL */
        static char verifier = 100;
        if (verifier != 100) {
                die("GOT error");/* GOT is not working, 
                                  * smth wrong with bss */
        }
#ifdef DEBUG
        pr_notice("This is a DEBUG version of kernel");
#endif                          /* DEBUG */

        pr_devel(KERN_CONT "COMPUTER INFO:");
        bootinfo(boot_magic, pcinfo);
        find_usable_RAM();
        early_init_interrupts();
        /* init kernel heap allocation */
        if(kalloc_init() == EXIT_SUCCESS) {
                pr_notice("Kalloc successfully initialized");
        } else {
                pr_err("Kalloc initialize failure, some kernel features \
                                cannot be used");
        }

#ifdef USE_VGA
        tui_init(OFFSET_FROM_TOP); // <----earliest init
        pr_warning
            (DEPRECATED "Note, that VGA mode is Legacy, used only for debbuging and text");
#elif defined NO_VIDEOMODE/* USE_VGA */
        tui_init(OFFSET_FROM_TOP); // <----earliest init
#endif /* USE VGA */
        if (init_video() == true) {
                tui_init(OFFSET_FROM_TOP); // <----earliest init
                pr_notice("videomode successfully started");
                sysfb.print_video_info();
                /* verifying bounds of display */
                sysfb.putchar_to('\0', text.rows - 1, text.columns - 1,
                            Red, Red, NOTRANSPARENT);
                sysfb.putchar_to('\0', 0, text.columns - 1, Red, Red, NOTRANSPARENT);
                sysfb.putchar_to('\0', text.rows - 1, 0, Red, Red, NOTRANSPARENT);

                printk(SMALL_SYS_EMBLEM);
        }
        pr_notice("\n\n\nTHUNDERSTORM %s Embedded system\n\
 COPYRIGHT Igor Muravyov (c) %s \n", TH_RELEASE, TH_YEARS);
        /* initializing interrupts */
        print_usable_RAM();
        if( detect_cpu() ) {
                /* err cpu not supported */
                kpanic("this cpu family is not supported");
        }
        printk("clearing kernel stacks: ");
        clear_kernel_stacks();
#ifdef ACPI_ALLOWED
        ACPI_STATUS acpi_status = AcpiInitializeTables(NULL, 1, 0);
        if(acpi_status != AE_OK) {
                pr_err(ACPI_MSG_ERROR "%s", acpi_strerror(acpi_status));
                kpanic(ACPI_MSG_ERROR);
        }

#endif
        init_interrupts();
                /*
        asm volatile (" exc: \n"
                      " mov $1, %rax\n"
                      " mov $2, %rdx\n"
                      " mov $3, %rcx\n"
                      " mov $8, %r8\n"
                      " mov $9, %r9\n"
                      " mov $10, %r10\n"
                      " mov $11, %r11\n"
                      " mov $-1, %rsi\n" " .quad 0xFFFFFFFFFFFFFFFF");*/
        local_irq_enable();
        while(1);
}
