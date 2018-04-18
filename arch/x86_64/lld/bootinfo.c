#define LOADER_NAME 2
#define MEMORY 4

#include <stdint.h>
#include <string.h>
#include <kstdio.h>
#include <TH/sysvars.h>
#include <TH/lld.h>
/**
 * bootinfo() function x86_64 port
 * recognizes computer system info via multiboot2 spec 
 * */
int loader_name(void *ebx)
{
  /* verify if it really is provided info */
  if(strlen((char*) (ebx + 2 * sizeof (uint32_t))) < 5 ) 
  {
    return -1;
  }
  kprintf("loader is %s\n", (char*) (ebx + 2 * sizeof (uint32_t)));
  return 0;
}

int memory(void *ebx)
{
  /* verify if it really is provided info */
  if(*(uint32_t*) (ebx + 1 * sizeof (uint32_t) ) != 16 )
  {
    return -1;
  }
  RAM.lowest = (uintptr_t *)(uintptr_t)(1024 * *(uint32_t*) (ebx + 2 * sizeof (uint32_t) ) );
  RAM.highest = (uintptr_t *)(uintptr_t)(1024 * *(uint32_t*) (ebx + 3 * sizeof (uint32_t) ) );
  kprintf("RAM lowestlimit: 0x%x K, highest: 0x%x K\n", 
      ( (uintptr_t)RAM.lowest ) / 0x400,
      ( (uintptr_t)RAM.highest ) / 0x400
      );
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
}
