#define LOADER_NAME 2
#define MEMORY 4
#define BOOTDEV 5
#define BOOTCMD 1
#define MODULES 3
#define VBEMODE 7
#define FRAMEBUFFER 8

#include <stdint.h>
#include <string.h>
#include <kstdio.h>
#include <TH/sysvars.h>
#include <TH/lld.h>
/**
 * bootinfo() function x86_64 port
 * recognizes computer system info via multiboot2 spec 
 * */
int vbe_mode(void *ebx)
{
  /* verify if it really is provided info */
  if(*(int*)(ebx + sizeof (uint32_t)) != 784)
  {
    return -1;
  }
  kprintf("vbe_mode: 0x%x", *(uint16_t*)(ebx + 2 * sizeof(uint32_t) ));
  return 0;
}
  
int loader_name(void *ebx)
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

int boot_cmd(void *ebx)
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

int boot_device(void *ebx)
{
  /* verify if it really is provided info */
  if( *(uint32_t*)(ebx + sizeof (uint32_t) ) != 20 ) 
  {
    return -1;
  }
  kprintf("BOOTDEV IS  0x%x\n", *(uint32_t*)(ebx + 2 * sizeof (uint32_t) ) );
  kprintf("PARTITION:  0x%x\n", *(uint32_t*)(ebx + 3 * sizeof (uint32_t) ) );
  kprintf("SUBPARTITION:  0x%x\n", *(uint32_t*)(ebx + 4 * sizeof (uint32_t) ) );
  return 0;
}


int memory(void *ebx)
{
  /* verify if it really is provided info */
  if(*(uint32_t*) (ebx + 1 * sizeof (uint32_t) ) != 16 )
  {
    return -1;
  }
  RAM.lowest = (uintptr_t *)(uintptr_t)
    (1024 * *(uint32_t*) (ebx + 2 * sizeof (uint32_t) ) );
  RAM.highest = 0x100000 /* 1M */
    + (uintptr_t *)(uintptr_t)
    (1024 * *(uint32_t*) (ebx + 3 * sizeof (uint32_t) ) );
  kprintf("RAM lowestlimit: 0x%x K, highest: 0x%x K\n", 
      ( (uintptr_t)RAM.lowest ) / 0x400,
      ( (uintptr_t)RAM.highest ) / 0x400
      );
  return 0;
}

int modules(void *ebx)
{
  /* verify if it really is provided info */
  if(strlen((char*)(ebx + 4 * sizeof (uint32_t) ) ) < 4
      || *(int32_t*)(ebx + sizeof (uint32_t) ) <= 2) 
  {
    return -1;
  }
  kprintf("module:  %s\n", (char*) (ebx +  4 * sizeof (uint32_t)  ) );
  return 0;
}

int framebuffer_info(void * ebx)
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



void bootinfo(void * ebx)
{
  /* total size of header*/
  uint32_t header_size=*((uint32_t*)ebx);
  while(header_size % sizeof (uint32_t) != 0) 
    header_size--;
  register void* bp;
  kprintf("total size of boot struct=%x\n",header_size);
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
      if(modules(bp) == 0)
      {
        times++;
      }
    }
  }
  kprintf("%d modules found\n", times);

  /* VBE MODE INFO */
  for(bp = ebx + header_size; bp>=ebx;  bp -= sizeof (uint32_t) )
  {
    if(*(uint32_t*)bp == VBEMODE)
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

}
