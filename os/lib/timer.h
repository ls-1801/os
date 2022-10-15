#ifndef _LIB_TIMER_H_
#define _LIB_TIMER_H_
#include <common.h>

typedef void (*timer_handler_t)(void);
typedef struct timer_handle *handle_t;

enum TimeUnit {
  ns = 0,
  mus = 1000,
  ms = 1000000,
};

handle_t timer_set(unsigned amt, enum TimeUnit, timer_handler_t, bool repeat);
void timer_cancel(handle_t);
void timer_reset(handle_t);
bool timer_is_canceled(handle_t);
bool timer_is_active(handle_t);

#endif /* _LIB_TIMER_H_ */