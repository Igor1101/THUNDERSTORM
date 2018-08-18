
#include <kstdio.h>
#include <TH/kernel.h>
#include <platform/acTHUNDERSTORM.h>
#include <platform/acenv.h>
#include <actypes.h>

void
AcpiError (
        const char *ModuleName,
        UINT32 LineNumber,
        const char *Format,
        ...)
{
        pr_err(ACPI_MSG_ERROR"file %s  line %s:", ModuleName, LineNumber);
        va_list args;
        va_start(args, Format);
        vprintk_default(Format, args);
        va_end(args);
}

