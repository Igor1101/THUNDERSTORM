#ifndef APIC_H
#define APIC_H

#include <stdint.h>

#define LAPIC_MSR 0x1B
#define LAPIC_AE (uint64_t)(1<<11)
#define LAPIC_BSC (uint64_t)(1<<8)

bool apic_present(void);
void apic_get_info_ACPI(void);
void boot_lapic_enable(void);

#endif /* APIC_H */
