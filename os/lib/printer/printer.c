#include "printer.h"

void putc(printer_t p, char c) { p->putc(p, c); }
void puts(printer_t p, const char *str) {
  while (*str != '\0') {
    p->putc(p, *str++);
  }
}