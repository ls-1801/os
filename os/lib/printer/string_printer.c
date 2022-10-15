#include "string_printer.h"

void string_putc(printer_t p, char c) {
  *(((struct StringPrinter *)p)->pos)++ = c;
}

void string_close(printer_t p) { *(((struct StringPrinter *)p)->pos) = '\0'; }

struct StringPrinter get_string_printer(char *buf) {
  struct StringPrinter ret = {{string_putc, string_close}, buf, buf};
  return ret;
}
