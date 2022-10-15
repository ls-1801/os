#include "printer.h"

void __putc(printer_t p, char c) { p->putc(p, c); }
void __puts(printer_t p, const char *str) {
  while (*str != '\0') {
    p->putc(p, *str++);
  }
}