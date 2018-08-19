#include <stdbool.h>
#include <kstdlib.h>
#include <kstring.h>
#include <stdarg.h>
#include <TH/sysvars.h>

#define PG_SIZE 4096
#define ALIGN_PG(X)     while( ( (X) % PG_SIZE ) != 0) X++
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
/**
 * int kalloc_init(void)
 * @return: SUCCESS if OK
 * FAILURE if memory cannot be allocated
 */
int kalloc_init(void)
{
        memset(RAM_pointers, 0, sizeof RAM_pointers);
        if(usable_ram_map[0].base_addr == NULL || 
        usable_ram_map[0].end_addr == NULL)
                return EXIT_FAILURE;
        current_mem_p = usable_ram_map[0].base_addr;
        end = usable_ram_map[0].end_addr;
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
        ALIGN_PG((uintptr_t)current_mem_p);
        RAM_pointers[RAM_pointers_used].end = current_mem_p;
        kmemset_show(
                        RAM_pointers[RAM_pointers_used].beginning,
                        0,
                        size
                   );
        RAM_pointers_used++;
        return ret;
}

/* TODO : reimplement alloc so this function could be easily implemented */
LIKELY void kfree(void *addr) {
        (void)addr;
}
