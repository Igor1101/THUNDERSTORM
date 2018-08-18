#ifndef ACPI_SUPPORT_H
#define ACPI_SUPPORT_H


#include <kstdio.h>
#include <TH/kernel.h>
#include <acpi.h>
#include <asm/memory_mapping.h>

#ifndef EXPORT_SYMBOL
#define EXPORT_SYMBOL
#endif
void
AcpiError (
        const char *ModuleName,
        UINT32 LineNumber,
        const char *Format,
        ...);

FORCE_INLINE void * AcpiOsMapMemory 
(
        ACPI_PHYSICAL_ADDRESS PhysicalAddress,
        ACPI_SIZE Length
        )
{
        return kmmap((void*)PhysicalAddress, Length);
}

#endif /*ACPI_SUPPORT_H */
