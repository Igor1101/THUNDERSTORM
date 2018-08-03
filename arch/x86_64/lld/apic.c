#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <x86_64/cpuid.h>
/** returns a 'true' value if the CPU supports APIC
 *  and if the local APIC hasn't been disabled in MSRs
 *  note that this requires CPUID to be supported.
 */
bool apic_present(void) 
{
   uint32_t eax, edx =0, ecx, ebx;
   ASSERT(__get_cpuid(1, &eax, &ebx, &ecx, &edx) == 1);
   return edx & CPUID_FEAT_EDX_APIC;
}
