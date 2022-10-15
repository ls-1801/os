#include "timer.h"
#include "../lib/bits.h"
//#include "../interrupts.h"
#define TIMER_BASE (0x7E00B000 + 0x400)
#define TIMER_CONTROL_ENABLE 7
#define TIMER_CONTROL_INTERRUPT 5
#define TIMER_CONTROL_PRESCALER 2
#define TIMER_CONTROL_COUNTER 1

struct TimerRegister {
  uint32_t load;
  const uint32_t value;
  uint32_t control;
  uint32_t irq_clear;
  const uint32_t raw_irq;
  const uint32_t masked_irq;
  uint32_t reload;
  uint32_t predivider;
  uint32_t free_running_counter;
};

struct Timer {
  volatile struct TimerRegister *tr;
  bool initialized;
};

static struct Timer timer = {(struct TimerRegister *)TIMER_BASE, false};

static inline void timer_init(timer_t t) {
  t->tr->control = 0;
  timer_enable(t, true);
  t->initialized = true;
}

timer_t get_timer() {
  if (!timer.initialized)
    timer_init(&timer);

  return &timer;
}

void timer_set_load(timer_t t, uint32_t load) { t->tr->load = load; }

void timer_enable(timer_t t, bool en) {
  if (en)
    SET_BIT(t->tr->control, TIMER_CONTROL_ENABLE);
  else
    UNSET_BIT(t->tr->control, TIMER_CONTROL_ENABLE);
}

void timer_set_predivider(timer_t t, uint16_t predivider) {
  t->tr->predivider = predivider;
}

void timer_set_prescaler(timer_t t, enum TimerPrescaler tp) {
  t->tr->control &= ~(0b11 << 2);
  t->tr->control |= (tp << 2);
}

void timer_enable_interrupt(timer_t t, bool en) {
  if (en)
    SET_BIT(t->tr->control, TIMER_CONTROL_INTERRUPT);
  else
    UNSET_BIT(t->tr->control, TIMER_CONTROL_INTERRUPT);
}

uint32_t timer_get_value(timer_t t) { return t->tr->value; }

void timer_set_counter(timer_t t, enum TimerCounter tc) {
  if (tc == _16_bit_counter)
    UNSET_BIT(t->tr->control, TIMER_CONTROL_COUNTER);
  else if (tc == _23_bit_counter)
    SET_BIT(t->tr->control, TIMER_CONTROL_COUNTER);
}

void timer_clear_irq(timer_t t) { t->tr->irq_clear = 1; }
