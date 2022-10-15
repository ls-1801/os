#ifndef _DRIVER_TIMER_H_
#define _DRIVER_TIMER_H_

#include <common.h>
#include <stdint.h>

typedef struct Timer *timer_t;

enum TimerPrescaler {
  _1_1_prescaler = 0b0,
  _1_16_prescaler = 0b1,
  _1_256_prescaler = 0b10,
};

enum TimerCounter {
  _16_bit_counter = 0,
  _23_bit_counter = 1,
};

timer_t get_timer();
uint32_t timer_get_value(timer_t);
void timer_enable(timer_t, bool);
void timer_enable_interrupt(timer_t, bool);
void timer_set_predivider(timer_t t, uint16_t predivider);
void timer_set_prescaler(timer_t t, enum TimerPrescaler tp);
void timer_set_counter(timer_t, enum TimerCounter);
void timer_clear_irq(timer_t);
void timer_set_load(timer_t, uint32_t);

#endif /* _DRIVER_TIMER_H_ */