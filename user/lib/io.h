#include <stddef.h>
#include <stdint.h>

void __kprintf(const char *format, ...);
void svc_puts(const char *str);
uint32_t svc_gets(char *buf, uint32_t length);