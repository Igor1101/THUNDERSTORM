#include <TH/lld.h>
#include <TH/sysinfo.h>
#include <TH/sysvars.h>
#include <TH/linker_info.h>
#include <kstdlib.h>
#include <kstdio.h>
#include <stdbool.h>

void print_RAM_info(void)
{
}

void select_RAM(void)
{
  /* for each region */
  for(int i=0; i<ram_entries; i++)
  {
    char * type;
    switch(ram_map[i].type)
    {
      case PHYSRAM: type = "PHYSICAL RAM";break;
      case ACPIRAM: type = "ACPI"; break;
      case PRESERVED: type = "MUST BE PRESERVED";break;
      default: type = "RESERVED";
    }
    kprintf("entry %d: addr 0x%x, length %dM, type %s",
        i, ram_map[i].base_addr,
        ram_map[i].length / (2 << 20),
        type
        );
    if((uintptr_t*)&kernel_phys_base >= ram_map[i].base_addr &&
        (uintptr_t*)&kernel_phys_base < ram_map[i + 1].base_addr)
    {
      kprintf("<-- kernel ( 0x%x )", &kernel_phys_base);
    }

    if((uintptr_t*)sysfb.addr >= ram_map[i].base_addr &&
        (uintptr_t*)sysfb.addr < ram_map[i + 1].base_addr)
    {
      kprintf("<-- fb ( 0x%x )", sysfb.virtaddr);
    }

    kputchar('\n');
  }
}
