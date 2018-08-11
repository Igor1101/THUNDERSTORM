#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

#include <compiler_opt.h>
#include <stdint.h>

#define LOADER_NAME 2
#define MEMORY 4 /**
                  * Basic memory info, 
                  * almost useless */
#define BOOTDEV 5
#define BOOTCMD 1
#define MODULES 3
#define VESAMODE 7
#define FRAMEBUFFER 8
#define MEMMAP 6                /* memory map */


__init static void multiboot2(void *pcinfo /* ebx */ );

/* We `ll not use this BIOS - dependent info */
FORCE_INLINE int vbe_mode(volatile void *ebx)
{
        /* verify if it really is provided info */
        if (*(int *)(ebx + sizeof(uint32_t)) != 784 /* fixed size */ ) {
                return EXIT_FAILURE;
        }
        kprintf("vbe_mode: 0x%x", *(uint16_t *) (ebx + 2 * sizeof(uint32_t)));
        return EXIT_SUCCESS;
}

FORCE_INLINE int loader_name(volatile void *ebx)
{
        /* verify if it really is provided info */
        if (strlen((char *)(ebx + 2 * sizeof(uint32_t))) < 5 ||
            *(int32_t *) (ebx + sizeof(uint32_t)) <= 0) {
                return EXIT_FAILURE;
        }
        kprintf("loader is %s\n", (char *)(ebx + 2 * sizeof(uint32_t)));
        return EXIT_SUCCESS;
}

FORCE_INLINE int boot_cmd(volatile void *ebx)
{
        /* verify if it really is provided info */
        if (strlen((char *)(ebx + 2 * sizeof(uint32_t))) < 4 ||
            *(int32_t *) (ebx + sizeof(uint32_t)) <= 0) {
                return EXIT_FAILURE;
        }
        strncpy(kcmdline, (char *)(ebx + 2 * sizeof(uint32_t)),
                sizeof(kcmdline));
        kprintf("kcmdline:  %s\n", kcmdline);
        return EXIT_SUCCESS;
}

FORCE_INLINE int boot_device(volatile void *ebx)
{
        /* verify if it really is provided info */
        if (*(uint32_t *) (ebx + sizeof(uint32_t)) != 20 /* fixed size */ ) {
                return EXIT_FAILURE;
        }
        kprintf("BOOTDEV IS  0x%x\n",
                *(uint32_t *) (ebx + 2 * sizeof(uint32_t)));
        kprintf("PARTITION:  0x%x\n",
                *(uint32_t *) (ebx + 3 * sizeof(uint32_t)));
        kprintf("SUBPARTITION:  0x%x\n",
                *(uint32_t *) (ebx + 4 * sizeof(uint32_t)));
        return EXIT_SUCCESS;
}

/* BASIC MEMORY INFORMATION */
FORCE_INLINE int memory(volatile void *ebx)
{
        /* verify if it really is provided info */
        if (*(uint32_t volatile *)(ebx + 1 * sizeof(uint32_t)) !=
            16 /* fixed size */ ) {
                return EXIT_FAILURE;
        }
        /* in kilobytes size of it*/
        RAM.lowest = (*(uint32_t *) (ebx + 2 * sizeof(uint32_t)));
        RAM.highest = *(volatile uint32_t *)(ebx + 3 * sizeof (uint32_t));
        kprintf("RAM lowest: %d K, highest: %d K\n",
                ((volatile uintptr_t)RAM.lowest),
                ((volatile uintptr_t)RAM.highest));
        return EXIT_SUCCESS;
}

FORCE_INLINE int modules_proc(volatile void *ebx, uint32_t num)
{
        /* verify if it really is provided info */
        if (strlen((char *)(ebx + 4 * sizeof(uint32_t))) < 3
            || *(int32_t *) (ebx + sizeof(uint32_t)) <= 2) {
                return EXIT_FAILURE;
        }
        modules[num].ext_name = (char *)(ebx + 4 * sizeof(uint32_t));
        modules[num].phys_addr_end = *(uint32_t *) (ebx + 3 * sizeof(uint32_t));
        modules[num].phys_addr = *(uint32_t *) (ebx + 2 * sizeof(uint32_t));
        kprintf("module:  %s, addr 0x%x to 0x%x\n", 
                        modules[num].ext_name,
                        modules[num].phys_addr,
                        modules[num].phys_addr_end
                        );
        return EXIT_SUCCESS;
}

FORCE_INLINE int memmap(volatile void *ebx)
  /* warning!: this function needs to be called after memory(); */
{
        /* verify if it really is provided info */
        if (*(int32_t *) (ebx + sizeof(int32_t)) < 16) {
                return EXIT_FAILURE;
        }
        auto volatile uint32_t size = *(uint32_t *) (ebx + sizeof(uint32_t));
        auto volatile uint32_t entry_size =
            *(uint32_t *) (ebx + 2 * sizeof(uint32_t));
        auto volatile uint32_t entry_version =
            *(uint32_t *) (ebx + 3 * sizeof(uint32_t));
        auto volatile void *mappointer = ebx + 4 * sizeof(uint32_t);
        auto volatile int rami = 0;
        for (; rami < MAX_RAM_ENTRIES; rami++) {
                if (mappointer >= ebx + size) {
                        break;
                }
                ram_map[rami].base_addr = *(uint64_t **) mappointer;
                ram_map[rami].length =
                    *(uint64_t *) (mappointer + sizeof(uint64_t));
                ram_map[rami].type =
                    *(uint32_t *) (mappointer + 2 * sizeof(uint64_t));
                mappointer += entry_size;
        }
        ram_entries = rami;
        kprintf("memmap:  ram_entries: %d entry version: %d\n",
                ram_entries, entry_version);
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
                        type = "RESERVED, must be preserved";
                        break;
                default:
                        type = "RESERVED";
                }
                kprintf("entry %d: addr 0x%x, length %dM, type %s \n",
                        i, ram_map[i].base_addr,
                        ram_map[i].length / 1024 / 1024, type);
        }
        return EXIT_SUCCESS;
}

FORCE_INLINE int framebuffer_info(volatile void *ebx)
{
        if (*(int32_t *) (ebx + 1 * sizeof(uint32_t)) <= 16) {
                return EXIT_FAILURE;
        }
        sysfb.addr = (uint64_t *) (*(uint64_t *) (ebx + 2 * sizeof(uint32_t)));
        sysfb.pitch = *(uint32_t *) (ebx + 2 * sizeof(uint32_t)
                                     + sizeof(uint64_t));
        sysfb.width = *(uint32_t *) (ebx + 3 * sizeof(uint32_t)
                                     + sizeof(uint64_t));
        sysfb.height = *(uint32_t *) (ebx + 4 * sizeof(uint32_t)
                                      + sizeof(uint64_t));
        sysfb.bpp = *(uint8_t *) (ebx + 5 * sizeof(uint32_t)
                                  + sizeof(uint64_t));
        sysfb.type = *(uint8_t *) (ebx + 5 * sizeof(uint32_t)
                                   + sizeof(uint64_t) + sizeof(uint8_t));
        kprintf("\nframebuffer : \n\taddr 0x%x \t\n\tpitch\
 0x%x \n\twidth 0x%x \n\theight 0x%x \n\tbpp 0x%x \n\ttype 0x%x\n",
                sysfb.addr, sysfb.pitch, sysfb.width,
                sysfb.height,
                sysfb.bpp,
                sysfb.type);
        /* only direct RGB is supported */
        if(sysfb.type == 1)
                sysfb.is_initialized = true;
        return EXIT_SUCCESS;
}

#endif /* MULTIBOOT2_H */
