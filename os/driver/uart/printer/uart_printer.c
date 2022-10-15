#include "uart_printer.h"
#include "../uart.h"
#include "../uart_io.h"
#include <common.h>

/**
 * Putc method essentially just puts the character into the TX buffer.
 * Once the printer is closed, the tx interrupts are enabled and the buffer
 * will be written to the data register. Once the buffer is empty tx interrupts
 * are disabled.
 */
static void nop_close() { uart_start_transmitting(get_uart()); }
static void async_uart_putc(printer_t p, char c) {
  UNUSED(p);
  uart_t u = get_uart();
  uart_putc(u, c);
}

static struct Printer printer_uart = {async_uart_putc, nop_close};

printer_t get_printer_uart() { return &printer_uart; }
