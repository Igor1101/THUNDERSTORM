#ifndef ACPI_SUPPORT_H
#define ACPI_SUPPORT_H


#include <kstdio.h>
#include <kstdlib.h>
#include <kstring.h>
#include <kctype.h>
#include <TH/kernel.h>
#include <acpi.h>
#include <asm/memory_mapping.h>

#ifndef EXPORT_SYMBOL
#define EXPORT_SYMBOL
#endif
#define AcpiOsPrintf(...) pr_cont(__VA_ARGS__)
/* 
 * ROOT POINTERS (initialized in bootinfo())
 * */
extern void* RSDP_1;
extern void* RSDP_2;

char* acpi_strerror(ACPI_STATUS st);



FORCE_INLINE void * AcpiOsMapMemory 
(
        ACPI_PHYSICAL_ADDRESS PhysicalAddress,
        ACPI_SIZE Length
        )
{
        return kmmap((void*)PhysicalAddress, Length);
}

FORCE_INLINE void
AcpiOsUnmapMemory (
        void *LogicalAddress,
        ACPI_SIZE Length
 )
{
        (void)Length;
        (void)LogicalAddress;
}

FORCE_INLINE void AcpiOsFree (void *Memory)
{
        kfree(Memory);
}

FORCE_INLINE void * AcpiOsAllocateZeroed (ACPI_SIZE Size)
{
        return kcalloc(Size);
}

FORCE_INLINE void * AcpiOsAllocate(ACPI_SIZE Size)
{
        return kcalloc(Size);
}

/*
ACPI_STATUS AcpiOsPurgeCache ( ACPI_CACHE_T *Cache){(void)Cache;return 0;}
*/

#endif /*ACPI_SUPPORT_H */
