/* 
 * It`s hard to understand what`s written on this file,
 * but trust me it successfully 
 * fetches information from multiboot header
 */
#define LOADER_NAME 2
#define MEMORY 4 /**
                  * Basic memory info, 
                  * almost useless */
#define BOOTDEV 5
#define BOOTCMD 1
#define MODULES 3
#define VESAMODE 7
#define FRAMEBUFFER 8
#define MEMMAP 6 /* memory map */

#include <stdint.h>
#include <kstring.h>
#include <kstdio.h>
#include <TH/sysvars.h>
#include <TH/lld.h>
#include <gcc_opt.h>
/**
 * bootinfo() function x86_64 port
 * recognizes computer system info via multiboot2 spec 
 * */
FORCE_INLINE int vbe_mode(volatile void *ebx)
{
  /* verify if it really is provided info */
  if(*(int*)(ebx + sizeof (uint32_t)) != 784/* fixed size */)
  {
    return -1;
  }
  kprintf("vbe_mode: 0x%x", *(uint16_t*)(ebx + 2 * sizeof(uint32_t) ));
  return 0;
}
  
FORCE_INLINE int loader_name(volatile void *ebx)
{
  /* verify if it really is provided info */
  if(strlen((char*) (ebx + 2 * sizeof (uint32_t))) < 5 || 
      *(int32_t*)(ebx + sizeof (uint32_t) ) <= 0) 
  {
    return -1;
  }
  kprintf("loader is %s\n", (char*) (ebx + 2 * sizeof (uint32_t) ) );
  return 0;
}

FORCE_INLINE int boot_cmd(volatile void *ebx)
{
  /* verify if it really is provided info */
  if(strlen((char*)(ebx + 2 * sizeof (uint32_t) ) ) < 4 || 
      *(int32_t*)(ebx + sizeof (uint32_t) ) <= 0) 
  {
    return -1;
  }
  kprintf("kcmdline:  %s\n", (char*) (ebx + 2 * sizeof (uint32_t) ) );
  return 0;
}

FORCE_INLINE int boot_device(volatile void *ebx)
{
  /* verify if it really is provided info */
  if( *(uint32_t*)(ebx + sizeof (uint32_t) ) != 20 /* fixed size */) 
  {
    return -1;
  }
  kprintf("BOOTDEV IS  0x%x\n", *(uint32_t*)(ebx + 2 * sizeof (uint32_t) ) );
  kprintf("PARTITION:  0x%x\n", *(uint32_t*)(ebx + 3 * sizeof (uint32_t) ) );
  kprintf("SUBPARTITION:  0x%x\n", *(uint32_t*)(ebx + 4 * sizeof (uint32_t) ) );
  return 0;
}


FORCE_INLINE int memory(volatile void *ebx)
{
  /* verify if it really is provided info */
  if(*(uint32_t volatile*) (ebx + 1 * sizeof (uint32_t) ) != 16 /* fixed size */)
  {
    return -1;
  }
  RAM.lowest = (uintptr_t *)(uintptr_t volatile)
    (1024 * *(uint32_t*) (ebx + 2 * sizeof (uint32_t) ) );
  RAM.highest = (uintptr_t * volatile)(0x100000 /* 1M */
    + (volatile uintptr_t)
    (1024 * *(volatile uint32_t*) (ebx + 3 * sizeof (uint32_t) ) ) );
  kprintf("RAM lowestlimit: 0x%x K, highest: 0x%x K\n", 
      ( (volatile uintptr_t)RAM.lowest ) / 0x400,
      ( (volatile uintptr_t)RAM.highest ) / 0x400
      );
  return 0;
}

FORCE_INLINE int modules_proc(volatile void *ebx, uint32_t num)
{
  /* verify if it really is provided info */
  if(strlen((char*)(ebx + 4 * sizeof (uint32_t) ) ) < 4
      || *(int32_t*)(ebx + sizeof (uint32_t) ) <= 2) 
  {
    return -1;
  }
  modules[num].ext_name = (char*) (ebx +  4 * sizeof (uint32_t)  );
  modules[num].phys_addr_end = *(uint32_t*) (ebx + 3 * sizeof (uint32_t));
  modules[num].phys_addr = *(uint32_t*) (ebx + 2 * sizeof (uint32_t) );
  kprintf("module:  %s\n",  modules[num].ext_name);
  return 0;
}

FORCE_INLINE int memmap(volatile void *ebx)
  /* warning!: this function needs to be called after memory(); */
{
  /* verify if it really is provided info */
  if( *(int32_t*)(ebx + sizeof (int32_t) ) < 16 ) 
  {
    return -1;
  }
  auto volatile uint32_t size = *(uint32_t*)(ebx + sizeof (uint32_t) );
  auto volatile uint32_t entry_size = *(uint32_t*)(ebx + 2 * sizeof (uint32_t) );
  auto volatile uint32_t entry_version = *(uint32_t*)(ebx + 3 * sizeof (uint32_t) );
  auto volatile void* mappointer = ebx + 4 * sizeof (uint32_t);
  auto volatile int rami=0;
  for(; rami<MAX_RAM_ENTRIES; rami++)
  {
    if(mappointer >= ebx + size)
    {
      break;
    }
    ram_map[rami].base_addr = *(uint64_t**)mappointer;
    ram_map[rami].length = *(uint64_t*)(mappointer + sizeof (uint64_t));
    ram_map[rami].type = *(uint32_t*)(mappointer + 2 * sizeof (uint64_t));
    mappointer += entry_size;
  }
  ram_entries = rami;
  kprintf("memmap:  ram_entries: %d entry version: %d\n",  
      ram_entries, entry_version);
  for(uint32_t i=0; i<ram_entries; i++)
  {
    char * type;
    switch(ram_map[i].type)
    {
      case PHYSRAM: type = "PHYSICAL RAM";break;
      case ACPIRAM: type = "ACPI";break;
      case PRESERVED: type = "RESERVED, must be preserved"; break;
      default: type = "RESERVED";
    }
    kprintf("entry %d: addr 0x%x, length %dM, type %s \n",
        i, ram_map[i].base_addr, 
        ram_map[i].length/1024/1024,
        type
        );
  }
  return 0;
}


FORCE_INLINE int framebuffer_info(volatile void * ebx)
{
  if(*(int32_t*) (ebx + 1 * sizeof (uint32_t)) <= 16)
  {
    return -1;
  }
  sysfb.addr = (uint64_t*)(*(uint64_t*)(ebx + 2 * sizeof (uint32_t) ) );
  sysfb.pitch = *(uint32_t*) (ebx + 2 * sizeof (uint32_t) 
      + sizeof (uint64_t) );
  sysfb.width = *(uint32_t*) (ebx + 3 * sizeof (uint32_t) 
      + sizeof (uint64_t) );
  sysfb.height = *(uint32_t*) (ebx + 4 * sizeof (uint32_t) 
      + sizeof (uint64_t) );
  sysfb.bpp = *(uint8_t*) (ebx + 5 * sizeof (uint32_t) 
      + sizeof (uint64_t) );
  sysfb.type = *(uint8_t*) (ebx + 5 * sizeof (uint32_t) 
      + sizeof (uint64_t) + sizeof (uint8_t) );
  kprintf("framebuffer addr 0x%x pitch 0x%x width 0x%x height 0x%x ", 
      sysfb.addr, sysfb.pitch, sysfb.width);
  kprintf("bpp 0x%x type 0x%x \n", 
      sysfb.bpp, sysfb.type, sysfb.width);
  return 0;
}



void bootinfo(volatile void * ebx)
{
  /* total size of header*/
  uint32_t header_size=*((uint32_t*)ebx);
  while(header_size % sizeof (uint32_t) != 0) 
    header_size--;
  register volatile void* bp;
  kprintf("total size of boot struct=0x%x\n", header_size);
  /* reserved */
  ebx += sizeof (uint32_t);
  /* parsing header info */
  /*Finding memory info */
  for(bp = ebx + header_size; bp>=ebx;  bp -= sizeof (uint32_t) )
  {
    if(*(uint32_t*)bp == MEMORY)
    {
      if(memory(bp) == 0)
      {
        break;
      }
    }
  }
  /* BOOTLOADER INFO */
  for(bp = ebx + header_size; bp>=ebx;  bp -= sizeof (uint32_t) )
  {
    if(*(uint32_t*)bp == LOADER_NAME)
    {
      if(loader_name(bp) == 0)
      {
        break;
      }
    }
  }
  /* BIOS and partition info */
  for(bp = ebx + header_size; bp>=ebx;  bp -= sizeof (uint32_t) )
  {
    if(*(uint32_t*)bp == BOOTDEV)
    {
      if(boot_device(bp) == 0)
      {
        break;
      }
    }
  }

  /* boot command line */
  for(bp = ebx + header_size; bp>=ebx;  bp -= sizeof (uint32_t) )
  {
    if(*(uint32_t*)bp == BOOTCMD)
    {
      if(boot_cmd(bp) == 0)
      {
        break;
      }
    }
  }

  /* modules */
  register uint32_t times=0;
  for(bp = ebx + header_size; bp>=ebx;  bp -= sizeof (uint32_t) )
  {
    if(*(uint32_t*)bp == MODULES)
    {
      if(modules_proc(bp, times) == 0)
      {
        times++;
      }
      if(times >= MAX_MODULES)
      {
        break;
      }
    }
  }
  module_entries = times;
  kprintf("%d modules found\n", times);

  /* VESA MODE INFO */
  for(bp = ebx + header_size; bp>=ebx;  bp -= sizeof (uint32_t) )
  {
    if(*(uint32_t*)bp == VESAMODE)
    {
      if(vbe_mode(bp) == 0)
      {
        break;
      }
    }
  }

  for(bp = ebx + header_size; bp>=ebx;  bp -= sizeof (uint32_t) )
  {
    if(*(uint32_t*)bp == FRAMEBUFFER)
    {
      if(framebuffer_info(bp) == 0)
      {
        break;
      }
    }
  }
  for(bp = ebx + header_size; bp>=ebx;  bp -= sizeof (uint32_t) )
  {
    if(*(uint32_t*)bp == MEMMAP)
    {
      if(memmap(bp) == 0)
      {
        break;
      }
    }
  }
}
