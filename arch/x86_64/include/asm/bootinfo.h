#ifndef BOOTINFO_H
#define BOOTINFO_H

#define MULTIBOOT1_MAGIC 0x2BADB002U

#define MULTIBOOT2_MAGIC 0x36D76289U

/**
 * boot info 
 * @bootmagic: boot spec verification
 * @pcinfo: pointer to provided information by bootloader
 * */
void bootinfo(uintptr_t bootmagic, void *pcinfo);

#endif				/* BOOTINFO_H */
