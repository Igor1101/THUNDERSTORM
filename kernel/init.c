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
#include <kstring.h>
#include <kstdlib.h>
#include <kstdio.h>
#include <stdbool.h>
#include <verify_kopt.h>
#include <TH/lld.h>
#include <TH/sysinfo.h>
#include <TH/sysvars.h>
#include <asm/int_handler.h>
#include <asm/cpu_management.h>
#include <asm/bootinfo.h>
#include <asm/serial.h>

/* declared vars */
struct RAM_INFO RAM;
struct Framebuffer sysfb;
struct RAM_MAP ram_map[MAX_RAM_ENTRIES];

VISIBLE int start_kernel(uintptr_t boot_magic, void *pcinfo)
{
#ifdef USE_SERIAL
	serial_disable_ints(SERIAL_MAIN);
	serial_configure_fifo(SERIAL_MAIN);
	serial_configure_line(SERIAL_MAIN);
#ifdef RELEASE
	/* baud rates setting faults on some x86_64 emulators */
	serial_configure_baud_rate(SERIAL_MAIN, 1);	// 115200 
#endif				/* RELEASE */
#endif				/* USE_SERIAL */
#ifdef USE_VGA
	tui_init();
	select_fgcolor(Red);
	kputs
	    ("Note, that VGA mode is Legacy, used only for debbuging and text");
	select_fgcolor(Default);
#endif				/* USE_VGA */
	static char verifier = 100;
	if (verifier != 100) {
		return (1);	/* GOT is not working, 
				   smth wrong with bss */
	}
#ifdef DEBUG
	select_fgcolor(Red);
	kputs("This is a DEBUG version of kernel");
	select_fgcolor(Default);
#endif				/* DEBUG */

	kputs("COMPUTER INFO:");
	bootinfo(boot_magic, pcinfo);
	if (init_video() == true) {
		tui_init();
		kputs("videomode successfully started");
		print_video_info();
		/* verifying bounds of display */
		kputchar_to('\0', text.rows - 1, text.columns - 1,
			    Red, Red, NOTRANSPARENT);
		kputchar_to('\0', 0, text.columns - 1, Red, Red, NOTRANSPARENT);
		kputchar_to('\0', text.rows - 1, 0, Red, Red, NOTRANSPARENT);

		select_fgcolor(Cyan);
		select_bgcolor(Blue);
		kputs(SMALL_SYS_EMBLEM);
		select_bgcolor(Black);
		select_fgcolor(Default);
	}
	kprintf("\n\n\nTHUNDERSTORM %s Embedded system\n\
 COPYRIGHT Igor Muravyov (c) %s \n", TH_RELEASE, TH_YEARS);
	print_RAM_info();
	/* initializing interrupts */
	set_exceptions();
	init_interrupts();
	asm volatile (" exc: \n"
		      " mov $1, %rax\n"
		      " mov $2, %rdx\n"
		      " mov $3, %rcx\n"
		      " mov $8, %r8\n"
		      " mov $9, %r9\n"
		      " mov $10, %r10\n"
		      " mov $11, %r11\n"
		      " mov $-1, %rsi\n" " .quad 0xFFFFFFFFFFFFFFFF");
	cpu_halt();
	while (1) ;
}
