#ifndef DRIVER_UART_H
#define DRIVER_UART_H
#include <common.h>

#define UART_BASE (0x7E201000)

typedef volatile struct UartRegister {
  /*0x00*/ uint32_t DR;
  /*0x04*/ uint32_t RSRECR;
  /*0x08*/ uint32_t padding1[4];
  /*0x18*/ uint32_t FR;
  /*0x1C*/ uint32_t padding2[1];
  /*0x20*/ uint32_t ILPR;
  /*0x24*/ uint32_t IBRD;
  /*0x28*/ uint32_t FBRD;
  /*0x2C*/ uint32_t LCRH;
  /*0x30*/ uint32_t CR;
  /*0x34*/ uint32_t IFLS;
  /*0x38*/ uint32_t IMSC;
  /*0x3C*/ uint32_t RIS;
  /*0x40*/ uint32_t MIS;
  /*0x44*/ uint32_t ICR;
  /*0x48*/ uint32_t DMACR;
  /*0x4C*/ uint16_t padding3[23];
  /*0x80*/ uint32_t ITCR;
  /*0x84*/ uint32_t ITIP;
  /*0x88*/ uint32_t ITOP;
  /*0x8C*/ uint32_t TDR;
} UartRegister;

enum FifoTriggerLevel {
  _1_8_FULL = 0b000,
  _1_4_FULL = 0b001,
  _1_2_FULL = 0b010,
  _3_4_FULL = 0b011,
  _7_8_FULL = 0b100,
};
void uart_set_rx_fifo_trigger_level(UartRegister *uart,
                                    enum FifoTriggerLevel level);

void uart_set_tx_fifo_trigger_level(UartRegister *uart,
                                    enum FifoTriggerLevel level);
void uart_clear_rx(UartRegister *uart);
void uart_clear_tx(UartRegister *uart);

bool uart_busy(UartRegister *uart);
bool uart_rx_empty(UartRegister *uart);
void uart_enable_rx_irq(UartRegister *uart, bool en);

void uart_enable_tx_irq(UartRegister *uart, bool en);

void uart_enable_fifo(UartRegister *uart, bool en);

bool uart_rx_full(UartRegister *uart);
bool uart_tx_full(UartRegister *uart);

void write_char(UartRegister *uart, char c);
char read_char(UartRegister *uart);

bool uart_rx_interrupt_pending(UartRegister *uart);
bool uart_tx_interrupt_pending(UartRegister *uart);

#endif /* DRIVER_UART_H */