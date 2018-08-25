#include <assert.h>
#include <acpi.h>
#include <stdint.h>
#include <stdbool.h>
#include <asm/kpanic.h>
#include <x86_64/cpuid.h>
#include <x86_64/APIC.h>

/* local APIC CPU 32-bit addr */
static uint64_t* boot_lapic_phys_addr = NULL;
static uint64_t* boot_lapic_virt_addr = NULL;

/** returns a 'true' value if the CPU supports APIC
 *  and if the local APIC hasn't been disabled in MSRs
 *  note that this requires CPUID to be supported.
 */
bool apic_present(void) 
{
        uint32_t eax, edx = 0, ecx, ebx;
        /* if this assert is failing 
        * it looks like CPUID is not supported */
        ASSERT(__get_cpuid(CPUID_GETFEATURES, &eax, &ebx, &ecx, &edx) == 1);
        return edx & CPUID_FEAT_EDX_APIC;
}

/*
 * bootstrap core LAPIC enable
 */
void boot_lapic_enable(void)
{
        uint64_t lapic_base = rdmsr(LAPIC_MSR);
        /*
        if(boot_lapic_virt_addr == NULL || 
                        boot_lapic_phys_addr == NULL) {
                return EXIT_FAILURE;
        }*/
        /* Are we sure we running on BOOTSTRAP CORE ? */
        pr_debug("LAPIC:0x%X", lapic_base);
        ASSERT( (lapic_base & LAPIC_BSC) == LAPIC_BSC );
        wrmsr(LAPIC_MSR, lapic_base | LAPIC_AE);
        printk("LAPIC IS INITIALIZED!");
}

void apic_get_info_ACPI(void)
{
        /* 
         * Assume that ACPI table is already loaded! 
         * Any ACPI errors here are fatal
         * */
        ACPI_TABLE_HEADER *APICp;
        ACPI_STATUS acstat = AcpiGetTable("APIC", 1, &APICp);
        if(acstat != AE_OK) {
                pr_crit(ACPI_MSG_ERROR "%s", acpi_strerror(acstat));
                kpanic(ACPI_MSG_ERROR);
        }
        /* findout LAPIC */
        boot_lapic_phys_addr =  (uint64_t*)(uint32_t*)((void*)APICp + 36);
        boot_lapic_virt_addr = 
                (uint64_t*)kmmap(boot_lapic_phys_addr, sizeof(boot_lapic_phys_addr));
        if(boot_lapic_virt_addr == NULL) {
                pr_crit("INSUFFICIENT DYNAMIC MEMORY");
                kpanic("INSUFFICIENT DYNAMIC MEMORY");
        }
        pr_debug(
                        "APIC table sig %s, len %d lapicaddr 0x%X", 
                        APICp -> Signature, APICp -> Length, 
                        boot_lapic_phys_addr 
                        );
}
