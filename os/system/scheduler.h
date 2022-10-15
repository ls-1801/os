#ifndef _SYSTEM_SCHEDULER_H_
#define _SYSTEM_SCHEDULER_H_
#include "thread.h"
#include <common.h>
typedef void (*threadFn)(void *);
void start_scheduling();
void start_thread(threadFn fn, void *arg, size_t arg_length);
void kill_active_thread();
void yield_current_thread();
thread_t get_active_thread();
#endif /* _SYSTEM_SCHEDULER_H_ */