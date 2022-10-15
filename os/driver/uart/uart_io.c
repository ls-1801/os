#include "uart_io.h"
#include "../../lib/bits.h"
#include "../../lib/ds/ringbuffer.h"
#include "../../system/interrupts.h"
#include "../../system/scheduler.h"
#include "../../system/uart_system_controller.h"

#include "uart.h"

#define TX_BUF_SIZE 4096
#define RX_BUF_SIZE 1024

static char TX_BUF[TX_BUF_SIZE];
static char RX_BUF[RX_BUF_SIZE];

struct Uart {
  UartRegister *reg;
  bool initialized;
  struct Buffer rxBuff;
  struct Buffer txBuff;
};

static struct Uart uart = {
    (UartRegister *)UART_BASE,
    false,
    {RX_BUF, RX_BUF, RX_BUF, 0, RX_BUF_SIZE},
    {TX_BUF, TX_BUF, TX_BUF, 0, TX_BUF_SIZE},
};

/**
 * RX Handler.
 *
 * Clear interrupt!
 * Read from the Data register until it is empty and copy into rxBuff.
 */
static void uart_rx_handler(uart_t uart) {
  uart_clear_rx(uart->reg);

  for (;;) {
    int c = uart_tst_getc(uart);
    if (c < 1)
      break;

    if (!uart_system_controller(c)) {
      buffer_put(&uart->rxBuff, (char)c);
    }
  }
}

/**
 * TX Handler.
 *
 * Clear interrupt!
 * As long the txBuffer is non empty. Try to write the character into uart. If
 * it fails we wait until the next interrupt.
 * Once the txBuffer is empty we can disable the tx interrupts.
 */
static void uart_tx_handler(uart_t uart) {
  uart_clear_tx(uart->reg);

  while (!buffer_empty(&uart->txBuff)) {
    // does not consume first char.
    char c = buffer_first(&uart->txBuff);
    if (uart_tst_putc(uart, c) < 0) {
      // Failed to write. We done here.
      return;
    }
    // Write success. Consume char.
    buffer_get(&uart->txBuff, &c, 1);
  }

  // Buffer is empty. No Longer Request Writes.
  uart_enable_tx_irq(uart->reg, false);
}

/**
 *
 * Uart IRQ Dispatch.
 *
 * Dispatching is rather simple. If RX IRQ enabled do the rx handler. If TX IRQ
 * enabled do the tx handler.
 *
 * Currently rx interrupts are never disabled. We always check if there is an
 * character to read and copy it into the RX buffer.
 * Tx interrupts on the other hand are only enabled if the TX buffer is non
 * empty.
 * */
void uart_irq_dispatch() {
  uart_t uart = get_uart();

  if (uart_rx_interrupt_pending(uart->reg)) {
    uart_rx_handler(uart);
  }

  if (uart_tx_interrupt_pending(uart->reg)) {
    uart_tx_handler(uart);
  }
}

/**
 * Initialization
 *
 * - Disable fifo because rx interrupts will only be triggered with 2 char in
 * fifo.
 * - Enable rx interrupts, as they should always be handled.
 * - Configure the Interrupt register and register a handler.
 */
static void uart_initialize(uart_t uart) {
  uart_enable_fifo(uart->reg, false);

  uart_enable_rx_irq(uart->reg, true);
  enable_irq(uart_int_pi);
  set_irq_interrupt_handler(uart_int_pi, uart_irq_dispatch);

  // uart_set_rx_fifo_trigger_level(uart, _1_8_FULL);
  uart->initialized = true;
}

uart_t get_uart() {
  if (!uart.initialized) {
    uart_initialize(&uart);
  }

  return &uart;
}

/**
 * tst functions either successfully write/read a character or return a negativ
 * value, but never block.
 */

int uart_tst_getc(uart_t uart) {
  if (uart_rx_empty(uart->reg))
    return -1;

  return (int)read_char(uart->reg);
}

int uart_tst_putc(uart_t uart, char c) {
  if (uart_tx_full(uart->reg))
    return -1;

  write_char(uart->reg, c);
  return c;
}

void uart_putc(uart_t uart, char c) {
  // Write Content into TX Buffer.
  // TX Interrupts have to be enabled manually. Which is done by the printer.
  buffer_put(&uart->txBuff, c);
}

void uart_start_transmitting(uart_t uart) {
  // Enables the TX Interrupts. There is no stop transmitting procedure since
  // that should be done when the buffer is empty and is handled my the tx
  // interrupt handler.
  uart_enable_tx_irq(uart->reg, true);
}

/**
 * sync methods will block until the character was written into the data
 * register. They do not require any interrupts to work and are essential for
 * debugging purpose.
 */

void uart_sync_putc(uart_t uart, char c) {
  while (uart_tx_full(uart->reg)) {
    NOP();
  }

  return write_char(uart->reg, c);
}

char uart_sync_getc(uart_t uart) {
  while (uart_rx_empty(uart->reg)) {
    NOP();
  }

  return read_char(uart->reg);
}

char uart_getc(uart_t uart) {
  while (buffer_empty(&uart->rxBuff)) {
    NOP();
  }
  char c;
  buffer_get(&uart->rxBuff, &c, 1);
  return c;
}

size_t uart_read(uart_t uart, char *buf, size_t length) {
  return buffer_get(&uart->rxBuff, buf, length);
}