/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef SYSVARS
#define SYSVARS

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <TH/lld.h>
#ifndef MAX_RAM_ENTRIES
#define MAX_RAM_ENTRIES 100     /*max amount of 
                                   ram entries */
#endif                          /* RAM_ENTRIES */
/*ram types */

#define PHYSRAM 1
#define ACPIRAM 3
#define PRESERVED 4

#define MAX_MODULES 20          /*max amount of modules */

        extern struct RAM_INFO
            /*
             * looks like this struct is useless in
             * x86_64 arch */
        {
                /* highest RAM size (in kbytes), loaded from bootloader */
                size_t highest;
                /* lowest RAM size, (in kbytes) loaded from bootloader */
                size_t lowest;
        } RAM;

/* bootloaded kernel modules structure */
        struct MODULES_struct {
                uint32_t phys_addr;
                uint32_t phys_addr_end;
                char *ext_name;
        } modules[MAX_MODULES];

        uint32_t module_entries;

        extern struct RAM_MAP {
                void *base_addr;
                uintptr_t length;
                uint32_t type;
        } ram_map[MAX_RAM_ENTRIES];
/* amount of ram entries currently stored in RAM_MAP*/
        extern uint32_t ram_entries;

        extern struct USABLE_RAM_MAP {
                void *base_addr;
                void *end_addr;
        } usable_ram_map[MAX_RAM_ENTRIES];

        extern uint32_t usable_ram_entries;


/* video info */
        extern struct Text_mode_pointer text;
        extern struct Framebuffer sysfb;


        int add_RAM(void* start, void* end);
        void *select_RAM(void);
        void print_usable_RAM(void);
        void find_usable_RAM(void);

#ifdef __cplusplus
}
#endif

#endif                          /* sysvars */
