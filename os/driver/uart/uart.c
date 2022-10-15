#include "uart.h"
#include "../../lib/bits.h"
#include "../../lib/ds/ringbuffer.h"
#include "../../system/interrupts.h"

#define FR_BUSY 3
#define FR_RXFE 4
#define FR_TXFF 5
#define FR_RXFF 6
#define FR_TXFE 7

#define LCRH_FEN 4
#define LCRH_WLEN_8_BIT 0b11 << 5

#define CR_RXE 9
#define CR_TXE 8
#define CR_UARTEN 1

#define TXIM 5
#define RXIM 4

#define TX_BUF_SIZE 1024
#define RX_BUF_SIZE 1024

void uart_set_rx_fifo_trigger_level(UartRegister *uart,
                                    enum FifoTriggerLevel level) {
  // Clear 3:5, Set 3:5 to FIFO Level
  uart->IFLS = (uart->IFLS & ~(0b111 << 3)) | (level << 3);
}

void uart_set_tx_fifo_trigger_level(UartRegister *uart,
                                    enum FifoTriggerLevel level) {
  // Clear 0:2, Set 0:2 to FIFO Level
  uart->IFLS = (uart->IFLS & ~(0b111)) | (level);
}

void uart_clear_rx(UartRegister *uart) { SET_BIT(uart->ICR, RXIM); }
void uart_clear_tx(UartRegister *uart) { SET_BIT(uart->ICR, TXIM); }

bool uart_busy(UartRegister *uart) { return CHECK_BIT(uart->FR, FR_BUSY); }

bool uart_tx_empty(UartRegister *uart) { return CHECK_BIT(uart->FR, FR_TXFE); }
bool uart_tx_full(UartRegister *uart) { return CHECK_BIT(uart->FR, FR_TXFF); }

bool uart_has_errror(UartRegister *uart) { return uart->DR & ~0xFF; }

bool uart_rx_empty(UartRegister *uart) { return CHECK_BIT(uart->FR, FR_RXFE); }
bool uart_rx_full(UartRegister *uart) { return CHECK_BIT(uart->FR, FR_RXFF); }

void uart_enable_rx_irq(UartRegister *uart, bool en) {
  if (en) {
    SET_BIT(uart->IMSC, RXIM);
  } else {
    UNSET_BIT(uart->IMSC, RXIM);
  }
}

void uart_enable_tx_irq(UartRegister *uart, bool en) {
  if (en) {
    SET_BIT(uart->IMSC, TXIM);
  } else {
    UNSET_BIT(uart->IMSC, TXIM);
  }
}

void uart_enable_fifo(UartRegister *uart, bool en) {
  if (en) {
    SET_BIT(uart->LCRH, LCRH_FEN);
  } else {
    UNSET_BIT(uart->LCRH, LCRH_FEN);
  }
}

#define RIS_TXRIS 5
#define RIS_RXRIS 4

bool uart_rx_interrupt_pending(UartRegister *uart) {
  return CHECK_BIT(uart->IMSC, RXIM);
}

bool uart_tx_interrupt_pending(UartRegister *uart) {
  return CHECK_BIT(uart->IMSC, TXIM);
}

void write_char(UartRegister *uart, char c) { uart->DR = c; }
char read_char(UartRegister *uart) { return (char)uart->DR; }
