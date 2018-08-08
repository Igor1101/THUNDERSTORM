/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <kstdlib.h>
#include <kstdio.h>
#include <stdbool.h>
#include <TH/lld.h>
#include <TH/sysinfo.h>
#include <TH/sysvars.h>
#include <TH/linker_info.h>
#include <asm/memory_mapping.h>


void find_usable_RAM(void)
{
        /* for each region */
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
                        ram_map[i].length / (1 << 20), type);
                if ((void *) & kernel_phys_base >= ram_map[i].base_addr &&
                    (void *) & kernel_phys_base < ram_map[i + 1].base_addr) {
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
                        /*modules addr info, find smallest and biggest addr */
                        uintptr_t biggest = modules[0].phys_addr_end;
                        uintptr_t smallest = modules[0].phys_addr;
                        for (uint32_t i = 0; i < module_entries; i++) {
                                kprintf
                                        (
                                         "\tmodule %s, 0x%x, 0x%x\n", 
                                        modules[i].ext_name,
                                        modules[i].phys_addr, 
                                        modules[i].phys_addr_end
                                        );
                                if(smallest > modules[i].phys_addr)
                                        smallest = modules[i].phys_addr;
                                if(biggest < modules[i].phys_addr_end)
                                        biggest = modules[i].phys_addr_end;
                        }
                        add_RAM(&init_kernel_text, (void*)smallest);
                        if(last_addr() 
                         > ( ram_map[i].base_addr + ram_map[i].length ) )
                                add_RAM((void*)biggest, 
                                    (ram_map[i].base_addr + ram_map[i].length) );
                        else
                                add_RAM((void*)biggest,  last_addr());
                        /* in case there are no modules */
                        if(module_entries == 0) {
                                if(last_addr() 
                                > ( ram_map[i].base_addr + ram_map[i].length ) )
                                        add_RAM(&init_kernel_text, 
                                        (ram_map[i].base_addr + ram_map[i].length) );
                                else
                                        add_RAM(&init_kernel_text,  last_addr());
                        }


                }
                else {
                        /* RAM entry without kernel */
                        if(ram_map[i].type == PHYSRAM 
                                        && (ram_map[i].base_addr + ram_map[i].length)
                                        > last_addr()) {
                                add_RAM(
                                       ram_map[i].base_addr, 
                                       last_addr()
                                       );
                        }
                        if(ram_map[i].type == PHYSRAM 
                                        && (ram_map[i].base_addr + ram_map[i].length)
                                        <= last_addr()) {
                                add_RAM(
                                       ram_map[i].base_addr, 
                                       (ram_map[i].base_addr + ram_map[i].length) 
                                       );
                        }

                }



                if ( sysfb.addr >= ram_map[i].base_addr &&
                    sysfb.addr < ram_map[i + 1].base_addr) {
                        kprintf("<-- fb ( 0x%x )\n", sysfb.virtaddr);
                }
                if ( last_addr() >= ram_map[i].base_addr &&
                     last_addr() < ram_map[i + 1].base_addr) {
                        kprintf("<-- last kernel addr 0x%x\n", last_addr());
                }

        }
}

void *select_RAM(void)
{
        return NULL;
}
