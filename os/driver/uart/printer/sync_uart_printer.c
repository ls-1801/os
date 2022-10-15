#include "sync_uart_printer.h"
#include "../uart_io.h"
#include <common.h>

static void nop_close() {}
static void sync_putc(printer_t p, char c) {
  UNUSED(p);
  uart_t u = get_uart();
  uart_sync_putc(u, c);
}

static struct Printer printer_uart_sync = {sync_putc, nop_close};

printer_t get_printer_uart_sync() { return &printer_uart_sync; }
