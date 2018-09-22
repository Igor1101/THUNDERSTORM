#include <stdbool.h>
#include <kstdlib.h>
#include <kstring.h>
#include <stdarg.h>
#include <TH/sysvars.h>
#include <TH/kernel.h>

#define PG_SIZE 4096
#define ALIGN_PG(X)     ALIGN(X, PG_SIZE)
#define pr_alloc(...) pr_debug("ALLOC_INIT:" __VA_ARGS__)
struct RAM_POINTERS
{
        void *beginning;
        void *end;
};
static struct RAM_POINTERS RAM_pointers[MAX_RAM_ALLOCS];
static uint32_t RAM_pointers_used = 0;
/* current alloc pointer */
static char* current_mem_p;
static char* end;
static int RAM_selected = 0;
static void*latest_addr = NULL;
/**
 * int kalloc_init(void)
 * @return: SUCCESS if OK
 * FAILURE if memory cannot be allocated
 */
int kalloc_init(void)
{
        ptrdiff_t reg_size = 
                usable_ram_map[0].end_addr 
                - usable_ram_map[0].base_addr;
        /* select biggest region */
        pr_alloc("finding out biggest RAM region");
        for(uint32_t RAM_s=0; RAM_s < usable_ram_entries; RAM_s++) {
                pr_alloc("reg %d size 0x%X", RAM_s, 
                        usable_ram_map[RAM_s].end_addr 
                        - usable_ram_map[RAM_s].base_addr);
                if(reg_size <
                        usable_ram_map[RAM_s].end_addr 
                        - usable_ram_map[RAM_s].base_addr) {
                        reg_size = 
                                usable_ram_map[RAM_s].end_addr 
                                - usable_ram_map[RAM_s].base_addr;
                        RAM_selected = RAM_s;
                        pr_alloc("reg %d bigger than last one", RAM_s);
                }
        }
        memset(RAM_pointers, 0, sizeof RAM_pointers);
        if(usable_ram_map[RAM_selected].base_addr == NULL || 
        usable_ram_map[RAM_selected].end_addr == NULL)
                return EXIT_FAILURE;
        current_mem_p = usable_ram_map[RAM_selected].base_addr;
        end = usable_ram_map[RAM_selected].end_addr;
        return EXIT_SUCCESS;
}

void *kcalloc(size_t size)
{
        if(size >= ( (uintptr_t)end - (uintptr_t)current_mem_p) ) {
                return NULL;
        }
        if(RAM_pointers_used >= MAX_RAM_ALLOCS) {
                return NULL;
        }
        void* ret = current_mem_p;
        RAM_pointers[RAM_pointers_used].beginning = current_mem_p;
        current_mem_p += size;
        current_mem_p = (char*)ALIGN_PG((uintptr_t)current_mem_p);
        RAM_pointers[RAM_pointers_used].end = current_mem_p;
        kmemset_show(
                        RAM_pointers[RAM_pointers_used].beginning,
                        0,
                        size
                   );
        RAM_pointers_used++;
        latest_addr = ret;
        return ret;
}

/* TODO : reimplement alloc and free func */
LIKELY void kfree(void *addr) 
{
        if(latest_addr == NULL)
                return;
        if(addr == latest_addr) {
                current_mem_p = latest_addr;
                pr_alloc("freed memory");
        }

}
