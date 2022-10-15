#include "io.h"
#include "uart/uart_io.h"

char getc() {
  uart_t u = get_uart();
  return uart_getc(u);
}

size_t read(char *buf, size_t length) {
  uart_t u = get_uart();
  return uart_read(u, buf, length);
}