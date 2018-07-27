/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <TH/lld.h>
#include <TH/sysinfo.h>
#include <TH/sysvars.h>
#include <TH/linker_info.h>
#include <kstdlib.h>
#include <kstdio.h>
#include <stdbool.h>
#include <memmapping.h>

void print_RAM_info(void)
{                               /* for each region */
        for (uint32_t i = 0; i < ram_entries; i++) {
                char *type;
                switch (ram_map[i].type) {
                case PHYSRAM:
                        type = "PHYSICAL RAM";
                        break;
                case ACPIRAM:
                        type = "ACPI";
                        break;
                case PRESERVED:
                        type = "MUST BE PRESERVED";
                        break;
                default:
                        type = "RESERVED";
                }
                kprintf("entry %d: addr 0x%x, length %dM, type %s\n",
                        i, ram_map[i].base_addr,
                        ram_map[i].length / (2 << 20), type);
                if ((uintptr_t *) & kernel_phys_base >= ram_map[i].base_addr &&
                    (uintptr_t *) & kernel_phys_base < ram_map[i + 1].base_addr)
                {
                        kprintf
                            ("<-- kernel:\n text: 0x%x,\n data: 0x%x,\
                             \n bss: 0x%x,\n init_text: 0x%x, \
                             \n init_bss: 0x%x, \n kernel_end: 0x%x\n",
                             &kernel_text, 
                             &kernel_data, 
                             &kernel_bss, 
                             &init_kernel_text,
                             &init_kernel_bss, 
                             &kernel_end
                             );
                        /*modules addr info */
                        for (uint32_t i = 0; i < module_entries; i++) {
                                kprintf
                                        (
                                         "\tmodule %s, 0x%x, 0x%x\n", 
                                        modules[i].ext_name,
                                        modules[i].phys_addr, 
                                        modules[i].phys_addr_end
                                        );
                        }
                }

                if ((uintptr_t *) sysfb.addr >= ram_map[i].base_addr &&
                    (uintptr_t *) sysfb.addr < ram_map[i + 1].base_addr) {
                        kprintf("<-- fb ( 0x%x )\n", sysfb.virtaddr);
                }
                if ((uintptr_t *) last_addr() >= ram_map[i].base_addr &&
                    (uintptr_t *) last_addr() < ram_map[i + 1].base_addr) {
                        kprintf("<-- last kernel addr 0x%x\n", last_addr());
                }

        }
}

void *select_RAM(void)
{
        return NULL;
}
