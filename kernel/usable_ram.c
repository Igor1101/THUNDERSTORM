/* usable RAM management */
#include <kstdlib.h>
#include <kstdio.h>
#include <stdbool.h>
#include <TH/kernel.h>
#include <TH/lld.h>
#include <TH/sysinfo.h>
#include <TH/sysvars.h>
#include <asm/memory_mapping.h>

struct USABLE_RAM_MAP usable_ram_map[MAX_RAM_ENTRIES];
uint32_t usable_ram_entries = 0;

int add_RAM(void* start, void *end)
{
        start = (void*)REAL_PG_ALIGN((uintptr_t)start);
        if(usable_ram_entries > MAX_RAM_ENTRIES)
                return EXIT_FAILURE;
        if(start + REAL_PG_SIZE >= end)
                return EXIT_FAILURE;
        if(start == NULL || end == NULL)
                return EXIT_FAILURE;
        usable_ram_map[usable_ram_entries].base_addr = start;
        usable_ram_map[usable_ram_entries].end_addr = end;
        usable_ram_entries++;
        return EXIT_SUCCESS;
}

void print_usable_RAM(void)
{
        printk("Usable RAM found:");
        for(uint32_t i = 0; i < usable_ram_entries; i++) {
                printk("entry %d: from 0x%x to 0x%x\n", i, 
                                usable_ram_map[i].base_addr,
                                usable_ram_map[i].end_addr
                       );
        }
}

