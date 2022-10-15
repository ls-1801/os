#ifndef DRIVER_UART_IO_H
#define DRIVER_UART_IO_H
#include <common.h>
typedef struct Uart *uart_t;

uart_t get_uart();
int uart_tst_getc(uart_t uart);

void uart_start_transmitting(uart_t uart);

int uart_tst_putc(uart_t uart, char c);

void uart_putc(uart_t dev, char c);
void uart_sync_putc(uart_t dev, char c);

char uart_sync_getc(uart_t dev);
char uart_getc(uart_t dev);
size_t uart_read(uart_t uart, char *buf, size_t length);

#endif /* DRIVER_UART_IO_H */