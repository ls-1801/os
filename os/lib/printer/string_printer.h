#ifndef _LIB_PRINTER_STRING_PRINTER_H_
#define _LIB_PRINTER_STRING_PRINTER_H_
#include "printer.h"

struct StringPrinter {
  struct Printer printer;
  char const *buf;
  char *pos;
};

struct StringPrinter get_string_printer(char *buf);

#endif /* _LIB_PRINTER_STRING_PRINTER_H_ */