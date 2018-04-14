#include "sys/types.h"
#include <stdint.h>

#define ITOA_BUF_SIZE 33

void itoa(char *buf, int base, int value);
void itoap(char *buf, size_t width, int base, uint32_t value);
void utoa(char *buf, uint32_t value);
