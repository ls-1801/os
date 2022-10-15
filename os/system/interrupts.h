#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include "system.h"
#include <common.h>

enum PeripheralInterrupts {
  uart_int_pi = 57,
  timer_pi = 64,
};

typedef void (*handler_t)();

void irq_enable_debug_mode(bool);
void enable_irq(enum PeripheralInterrupts pi);
void disable_irq(enum PeripheralInterrupts pi);
void set_irq_interrupt_handler(enum PeripheralInterrupts pi, handler_t handler);

#endif
