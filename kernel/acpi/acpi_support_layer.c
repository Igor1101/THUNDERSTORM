
#include <assert.h>
#include <kstdio.h>
#include <TH/kernel.h>
#include <TH/acpi_support.h>
#include <platform/acTHUNDERSTORM.h>
#include <platform/acenv.h>
#include <actypes.h>

#ifdef __x86_64__
#include <x86_64/cpu_management.h>
#endif

char* acpi_strerror(ACPI_STATUS st)
{
        /* We are not giving garbage to ACPI, aren`t we ? ;) */
        ASSERT(st == AE_BAD_PARAMETER);
        switch(st) {
                case AE_OK: 
                        return "OP SUCCESSFULL";
                case AE_NOT_FOUND:
                        return "NOT FOUND";
                case AE_NO_MEMORY:
                        return "INSUFFICIENT DYNAMIC MEMORY TO COMPLETE THE OPERATION";
                case AE_NO_ACPI_TABLES:
                        return "NO ACPI TABLES";
                default:
                        return "UNKNOWN ERROR";
        }
}

ACPI_PHYSICAL_ADDRESS AcpiOsGetRootPointer()
{
	ACPI_PHYSICAL_ADDRESS  Ret = 0;
        /* if it`s old x64 system (we assume we don`t know it) */
	if(AcpiFindRootPointer(&Ret) == AE_OK) {
	        return Ret;
        }
        AcpiOsPrintf("RSDP could not be found automatically\n ");
        if(RSDP_2 != NULL) {
                AcpiOsPrintf("RSDPv2\n ");
                return (ACPI_PHYSICAL_ADDRESS)RSDP_2;
        }
        if(RSDP_1 != NULL) {
                AcpiOsPrintf("RSDPv1\n ");
                return (ACPI_PHYSICAL_ADDRESS)RSDP_1;
        }
        pr_crit("RSDP not found");
        return 0;
}
ACPI_STATUS AcpiOsInitialize(void){return 0;}
ACPI_STATUS AcpiOsTerminate(void){return 0;}


void AcpiOsVprintf(const char *Format, va_list args)
{
        vprintk_emit(0, LOGLEVEL_DEFAULT, NULL, 0, Format, args);
}

ACPI_STATUS AcpiOsWaitSemaphore ( 
                ACPI_SEMAPHORE Handle, 
                UINT32 Units, 
                UINT16 Timeout)
{
        (void)Handle;
        (void)Units;
        (void)Timeout;
        return 0;
}

ACPI_THREAD_ID AcpiOsGetThreadId(void){return 0;}
void AcpiOsDeleteLock ( ACPI_HANDLE Handle){(void)Handle;}
ACPI_STATUS AcpiOsDeleteSemaphore ( ACPI_SEMAPHORE Handle)
{(void)Handle; return 0;}
ACPI_STATUS AcpiOsCreateSemaphore ( 
        UINT32 MaxUnits,
        UINT32 InitialUnits,
        ACPI_SEMAPHORE *OutHandle
        )
{
        (void)MaxUnits;
        (void)InitialUnits;
        (void)OutHandle;
        return 0;
}

ACPI_STATUS AcpiOsCreateLock (
        ACPI_SPINLOCK *OutHandle
        )
{
        (void)OutHandle;
        return 0;
}

void
AcpiOsReleaseLock (
        ACPI_SPINLOCK Handle,
        ACPI_CPU_FLAGS Flags
        )
{
        (void)Handle;
        (void)Flags;
}


ACPI_CPU_FLAGS
AcpiOsAcquireLock (
        ACPI_SPINLOCK Handle
        )
{
        (void)Handle;
        return 0;
}

ACPI_STATUS AcpiOsPhysicalTableOverride (
        ACPI_TABLE_HEADER *ExistingTable,
        ACPI_PHYSICAL_ADDRESS *NewAddress,
        UINT32 *NewTableLength
        )
{
        (void)ExistingTable;
        (void)NewAddress;
        *NewTableLength = 0;
        return 0;
}

ACPI_STATUS AcpiOsSignalSemaphore (
        ACPI_SEMAPHORE Handle,
        UINT32 Units
        )
{
        (void)Handle;
        (void)Units;
        return 0;
}

ACPI_STATUS AcpiOsTableOverride (
        ACPI_TABLE_HEADER *ExistingTable,
        ACPI_TABLE_HEADER **NewTable
        )
{
        (void)ExistingTable;
        *NewTable = NULL;
        return 0;
}

ACPI_STATUS AcpiOsPredefinedOverride (
        const ACPI_PREDEFINED_NAMES *PredefinedObject,
        ACPI_STRING *NewValue
        )
{
        (void)PredefinedObject;
        (void)NewValue;
        return 0;
}

ACPI_STATUS AcpiOsWritePort (
        ACPI_IO_ADDRESS Address,
        UINT32 Value,
        UINT32 Width/*in bits */
        )
{
#ifdef __x86_64__
        outb(Address, (Value & ( (-1)>>( 32 - Width ) ) ) );
#endif
        return 0;
}

ACPI_STATUS AcpiOsReadPort (
        ACPI_IO_ADDRESS Address,
        UINT32 *Value,
        UINT32 Width
        )
{
        (void)Width;
        *Value = (uint32_t)inb(Address);
        return 0;
}

void AcpiOsStall (UINT32 Microseconds){(void)Microseconds;}

ACPI_STATUS AcpiOsEnterSleep (
        UINT8   SleepState,
        UINT32  RegaValue,
        UINT32  RegbValue
        )
{
        (void)SleepState;
        (void)RegaValue;
        (void)RegbValue;
        return 0;
}

ACPI_STATUS AcpiOsWriteMemory (
        ACPI_PHYSICAL_ADDRESS Address,
        UINT64 Value,
        UINT32 Width
        )
{
        if(Width == 8 * sizeof(uint64_t))
                *(uint64_t*)Address = (uint64_t)Value;
        if(Width == 8 * sizeof(uint32_t))
                *(uint32_t*)Address = (uint32_t)Value;
        if(Width == 8 * sizeof(uint16_t))
                *(uint16_t*)Address = (uint16_t)Value;
        if(Width == 8 * sizeof(uint8_t))
                *(uint8_t*)Address = (uint8_t)Value;
        return 0;
}

ACPI_STATUS AcpiOsReadMemory (
        ACPI_PHYSICAL_ADDRESS Address,
        UINT64 *Value,
        UINT32 Width
        )
{
        if(Width == 8 * sizeof(uint64_t))
                *Value = *(uint64_t*)Address;
        if(Width == 8 * sizeof(uint32_t))
                *Value = *(uint32_t*)Address;
        if(Width == 8 * sizeof(uint16_t))
                *Value = *(uint16_t*)Address;
        if(Width == 8 * sizeof(uint8_t))
                *Value = *(uint8_t*)Address;
        return 0;
}

ACPI_STATUS AcpiOsReadPciConfiguration(
        ACPI_PCI_ID *PciId,
        UINT32 Reg,
        UINT64 *Value,
        UINT32 Width
        )
{
        return 0;
}

void AcpiOsSleep(UINT64 Milliseconds){(void)Milliseconds;}

ACPI_STATUS AcpiOsWritePciConfiguration (
        ACPI_PCI_ID             *PciId,
        UINT32                  Reg,
        UINT64                  Value,
        UINT32                  Width
        ){return 0;}

ACPI_STATUS AcpiOsSignal (
        UINT32 Function,
        void *Info
        )
{return 0;}
UINT64
AcpiOsGetTimer (
    void){return 0;}

void AcpiOsWaitEventsComplete ( void){};
ACPI_STATUS AcpiOsInstallInterruptHandler
(
        UINT32 InterruptLevel,
        ACPI_OSD_HANDLER Handler,
        void *Context
        ){return 0;}
ACPI_STATUS
AcpiOsRemoveInterruptHandler (
    UINT32                  InterruptNumber,
    ACPI_OSD_HANDLER        ServiceRoutine){return 0;}
ACPI_STATUS AcpiOsExecute (
    ACPI_EXECUTE_TYPE       Type,
    ACPI_OSD_EXEC_CALLBACK  Function,
    void                    *Context){return 0;}

