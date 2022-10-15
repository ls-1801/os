#include "timer.h"
#include "../driver/timer.h"
#include "../system/interrupts.h"

struct timer_handle {
  bool canceled;
  bool done;
  timer_t timer;
  timer_handler_t handler;
  bool repeat;
  uint32_t amt;
  enum TimeUnit unit;
};

static struct timer_handle timer = {false, true, NULL, NULL, false, 0, ms};

static void timer_handler() {
  timer_clear_irq(timer.timer);
  timer_enable_interrupt(timer.timer, false);
  disable_irq(timer_pi);
  timer.done = true;

  if (timer.canceled)
    return;

  if (timer.handler)
    timer.handler();

  if (timer.repeat)
    timer_set(timer.amt, timer.unit, timer.handler, timer.repeat);
}

/**
 * 250 MHZ -> 1 every 4 ns
 * 1/1000 predivider -> 1 every 4 mus
 *
 * 1/16 prescaler -> 1 every 64 ns
 * 1/1000 predivider -> 1 every 64 mus
 *
 * 1/256 prescaler -> 1 every 1.024 mus
 * 1/1000 predivider -> 1 every 1.024 ms
 *
 */
static void configure_timer(uint32_t amt, enum TimeUnit tu, timer_t t) {
  if (tu == ms) {
    amt *= 1000;
    tu = 1000;
  }

  timer_set_predivider(t, tu);
  timer_set_counter(t, _16_bit_counter);

  if (amt > 1000) {
    timer_set_prescaler(t, _1_256_prescaler);
    timer_set_load(t, amt / 1024u);
  } else if (amt > 64) {
    timer_set_prescaler(t, _1_16_prescaler);
    timer_set_load(t, amt / 64u);
  } else {
    timer_set_prescaler(t, _1_1_prescaler);
    timer_set_load(t, amt / 64u);
  }
}

handle_t timer_set(unsigned amt, enum TimeUnit unit, timer_handler_t handler,
                   bool repeat) {
  if (!timer.timer)
    timer.timer = get_timer();

  timer.done = false;
  timer.amt = amt;
  timer.unit = unit;
  timer.repeat = repeat;
  timer.handler = handler;
  timer.canceled = false;

  enable_irq(timer_pi);
  timer_enable_interrupt(timer.timer, true);

  configure_timer(amt, unit, timer.timer);
  set_irq_interrupt_handler(timer_pi, timer_handler);

  return &timer;
}

void timer_reset(handle_t handle) {
  timer_set(handle->amt, handle->unit, handle->handler, handle->repeat);
}

bool timer_is_canceled(handle_t handle) { return handle->canceled; }
bool timer_is_active(handle_t handle) {
  return !handle->canceled && !handle->done;
}

void timer_cancel(handle_t handle) {
  if (handle != NULL)
    handle->canceled = true;

  timer_enable_interrupt(handle->timer, false);
  disable_irq(timer_pi);
}