/* 
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 * It`s hard to understand what`s written on this file,
 * but trust me it successfully 
 * fetches information from multiboot header
 */

#include <compiler_opt.h>
#include <stdint.h>
#include <stdbool.h>
#include <kstring.h>
#include <kstdlib.h>
#include <kstdio.h>
#include <TH/sysvars.h>
#include <TH/lld.h>
#include <TH/die.h>
#include <TH/kcmdline.h>
#include <asm/bootinfo.h>
#include "multiboot2.h"

char kcmdline[KCMDLINE_SIZE];

uint32_t ram_entries;

/**
 * bootinfo() function x86_64 port
 * recognizes computer system info via multiboot2 spec 
 * */
__init void bootinfo(uintptr_t bootmagic, void *pcinfo)
{
        switch (bootmagic) {
        case MULTIBOOT1_MAGIC:
                printk("MULTIBOOT1 recognized, but still not supported");
                die("MULTIBOOT1 recognized, but still not supported");
        case MULTIBOOT2_MAGIC:
                multiboot2(pcinfo);
                break;
        default:
                printk("Bootloader is not supported, giving up");
                die("Bootloader is not supported");
        }
}

__init static void multiboot2(void *pcinfo /* ebx */ )
{
        /* array shows if info is processed */
        static bool is_done[30];
        memset(is_done, false, sizeof (is_done) );
        /* total size of header */
        uint32_t header_size = *((uint32_t *) pcinfo);
        while (header_size % sizeof(uint32_t) != 0)
                header_size--;
        register volatile void *bp;
        printk("total size of boot struct=0x%x\n", header_size);
        /* reserved */
        pcinfo += sizeof(uint32_t);
        register uint32_t modules_times = 0;

        /* parsing header info */
        for (bp = pcinfo + header_size; bp >= pcinfo; bp -= sizeof(uint32_t)) {
                switch(*(uint32_t *) bp ) {
                        case MEMORY:
                                if(is_done[MEMORY] != true) {
                                        if(memory(bp) == EXIT_SUCCESS)
                                                is_done[MEMORY] = true;
                                }
                                break;
                        case LOADER_NAME:
                                if(is_done[LOADER_NAME] != true) {
                                        if(loader_name(bp) == 0)
                                                is_done[LOADER_NAME] = true;
                                }
                                break;
                        case BOOTDEV:
                                if(is_done[BOOTDEV] != true) {
                                        if(boot_device(bp) == EXIT_SUCCESS)
                                                is_done[BOOTDEV] = true;
                                }
                                break;
                        case BOOTCMD:
                                if(is_done[MEMORY] != true) {
                                        if(boot_cmd(bp) == EXIT_SUCCESS)
                                                is_done[BOOTCMD] = true;
                                }
                                break;
                        case VESAMODE:
                                if(is_done[MEMORY] != true) {
                                        if(vbe_mode(bp) == EXIT_SUCCESS)
                                                is_done[VESAMODE] = true;
                                }
                                break;
                        case FRAMEBUFFER:
                                if(is_done[MEMORY] != true) {
                                        if(framebuffer_info(bp) == EXIT_SUCCESS)
                                                is_done[FRAMEBUFFER] = true;
                                }
                                break;

                        case MEMMAP:
                                if(is_done[MEMMAP] != true) {
                                        if(memmap(bp) == EXIT_SUCCESS)
                                                is_done[MEMMAP] = true;
                                }
                                break;
                        case MODULES:
                                if(is_done[MODULES] != true) {
                                        if(modules_proc(bp, modules_times)
                                                        == EXIT_SUCCESS) {
                                                modules_times++;
                                        }
                                        if (modules_times >= MAX_MODULES) {
                                                is_done[MODULES] = true;
                                        }
                                }
                                break;

                }
        }
        module_entries = modules_times;
}
