#include "string_printer.h"

void __string_putc(printer_t p, char c) {
  *(((struct StringPrinter *)p)->pos)++ = c;
}

void __string_close(printer_t p) { *(((struct StringPrinter *)p)->pos) = '\0'; }

struct StringPrinter __get_string_printer(char *buf) {
  struct StringPrinter ret = {{__string_putc, __string_close}, buf, buf};
  return ret;
}
