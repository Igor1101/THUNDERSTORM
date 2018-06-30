#include <TH/lld.h>
#include <kstdio.h>
#define EXC_START \
  select_fgcolor(Red);\
  kprintf("TH EXCEPTION:");\
  select_fgcolor(Default);

void set_exceptions(void);
void init_interrupts(void);
