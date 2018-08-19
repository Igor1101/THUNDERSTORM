
#include <kstdio.h>
#include <TH/kernel.h>
#include <TH/acpi_support.h>
#include <platform/acTHUNDERSTORM.h>
#include <platform/acenv.h>
#include <actypes.h>

#ifdef __x86_64__
#include <x86_64/cpu_management.h>
#endif

ACPI_PHYSICAL_ADDRESS AcpiOsGetRootPointer()
{
	ACPI_PHYSICAL_ADDRESS  Ret;
        if(RSDP_2 != NULL)
                return (ACPI_PHYSICAL_ADDRESS)RSDP_2;
        if(RSDP_1 != NULL)
                return (ACPI_PHYSICAL_ADDRESS)RSDP_1;
        /* last chance to get it! */
	Ret = 0;
	AcpiFindRootPointer(&Ret);
	return Ret;
}
ACPI_STATUS AcpiOsInitialize(void){return 0;}
ACPI_STATUS AcpiOsTerminate(void){return 0;}

void ACPI_INTERNAL_VAR_XFACE
AcpiOsPrintf (
const char *Format,
...
)
{
        va_list args;

        va_start(args, Format);
        vprintk_default(Format, args);
        va_end(args);
}
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
        (void)NewTableLength;
        return NULL;
}

ACPI_STATUS AcpiOsSignalSemaphore (
        ACPI_SEMAPHORE Handle,
        UINT32 Units
        )
{
        (void)Handle;
        (void)Units;
        return NULL;
}

ACPI_STATUS AcpiOsTableOverride (
        ACPI_TABLE_HEADER *ExistingTable,
        ACPI_TABLE_HEADER **NewTable
        )
{
        (void)ExistingTable;
        (void)NewTable;
        return NULL;
}

ACPI_STATUS AcpiOsPredefinedOverride (
        const ACPI_PREDEFINED_NAMES *PredefinedObject,
        ACPI_STRING *NewValue
        )
{
        (void)PredefinedObject;
        (void)NewValue;
        return NULL;
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
        return NULL;
}

ACPI_STATUS AcpiOsReadPort (
        ACPI_IO_ADDRESS Address,
        UINT32 *Value,
        UINT32 Width
        )
{
        (void)Width;
        static unsigned char in;
        Value = &in;
#ifdef __x86_64__
        in = inb(Address);
#endif
        return NULL;
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
        return NULL;
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
        static uint64_t value;
        Value = &value;
        if(Width == 8 * sizeof(uint64_t))
                value = *(uint64_t*)Address;
        if(Width == 8 * sizeof(uint32_t))
                value = *(uint32_t*)Address;
        if(Width == 8 * sizeof(uint16_t))
                value = *(uint16_t*)Address;
        if(Width == 8 * sizeof(uint8_t))
                value = *(uint8_t*)Address;
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

