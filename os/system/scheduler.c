#include "../lib/ds/static_linked_list.h"
#include "../lib/io.h"
#include "../lib/memory.h"
#include "../lib/timer.h"
#include "thread.h"
#include <common.h>

#define SHOW_CONTEXT_SWITCH() kprintf("\n")
#define QUANTUM_IN_MILLIS 1000

static thread_t active_thread = NULL;
static handle_t timer_handle = NULL;
static thread_t idle_thread = NULL;
static bool scheduler_started = false;

// List stores just the reference to the thread.
// Threads are stored in the thread.c as the thread implementation is unknown in
// here. MAX_THREADS is the Maxiumum amount of supported Threads including the
// Idle Thread. The Idle Thread however is not included in the list hence the
// list beeing 1 smaller.
MK_LIST(thread_list, thread_t, MAX_THREADS - 1);

static thread_t get_next_thread() {
  if (sll_length(thread_list) == 0)
    return idle_thread;

  return *(thread_t *)sll_get_first(thread_list);
}

thread_t get_active_thread() { return active_thread; }

/**
 * Timer is active and on auto repeat from the start.
 * schedule() does not change the state of the timer.
 * Timer is either cancled, reactived or restarted
 *
 * start_thread()
 * - if the timer is not currently active and adding a new thread would add a
 *   SECOND thread to the queue, the timer is reactived.
 * - if the thread would be the only Thread, the timer is set without repeat to
 *   cause a single schedule call. Since there is only one thread (excluding the
 *   idle Thread) no further scheduling should be required until a second thread
 *   is added.
 *
 * kill_active_thread()
 * - if killing the active thread would cause the queue to have just a single or
 *   no thread (excluding idle). The timer is canceld.
 * - otherwise we choose the next thread in restart the timer.
 *
 * yield_active_thread()
 * - if no other thread is available, no timer should be active.
 * - if there is another thread we do a context switch and reset the timer.
 **/

/**
 * Timer Handler callback that schedules the next Thread.
 * - Chooses the next Thread from the start of the list.
 * - If there is no thread in the queue the idle thread is choosen.
 * - At the end of scheduling, if the same thread is scheduled as the previous
 *   thread no context switch is done, but the TCB is updated.
 **/
void schedule() {
  kprintf("!");
  if (!scheduler_started)
    return;

  if (!idle_thread) {
    // No Active Thread. This happens when the scheduler is invoked for the
    // first time. We create a new Thread. The Initial Thread becomes the idle
    // thread.
    idle_thread = allocate_thread();
    active_thread = idle_thread;
    save_thread(idle_thread);
  }

  sll_rotate(thread_list);
  thread_t next_thread = get_next_thread();

  if (next_thread != active_thread) {
    save_thread(active_thread);
    active_thread = next_thread;
    load_thread(active_thread);
    SHOW_CONTEXT_SWITCH();
  } else {
    save_thread(active_thread);
  }
}

void kill_active_thread() {
  if (active_thread == idle_thread) {
    return; // Not Gonna Kill the Idle Thread. Timer should already be canceled.
  }

  thread_t at;
  sll_pop_first(thread_list, &at);
  free_thread(at);

  if (sll_length(thread_list) > 1) {
    // Reset Timer. So the new thread gets its full quantum.
    timer_reset(timer_handle);
  } else {
    // Just a single Thread left in queue. No further CS required.
    timer_cancel(timer_handle);
  }

  active_thread = get_next_thread();
  load_thread(active_thread);
  SHOW_CONTEXT_SWITCH();
}

void yield_current_thread() {
  save_thread(active_thread);

  if (sll_length(thread_list) < 2) {
    return; // No other Thread available. Yield is ignored. Timer should already
            // be disabled.
  }

  // Reset timer so the next thread gets its full quantum.
  timer_reset(timer_handle);
  sll_rotate(thread_list);
  active_thread = get_next_thread();
  load_thread(active_thread);
  SHOW_CONTEXT_SWITCH();
}

/**
 * Starts a new Thread.
 * - Allocate a Thread.
 * - Initialize the Thread.
 * - Add thread into list.
 *
 * Thread will be added into the end of the list.
 **/
void start_thread(threadFn fn, void *arg, size_t arg_length) {
  // Allocate Thread
  thread_t t = allocate_thread();

  if (t == NULL) {
    kprintf("Could not allocate a thread.\n");
    return;
  }

  // Initialize Thread
  initialize_thread(t);
  // Copy aligned data to the threads stack
  void *sp = get_thread_sp(t);
  sp -= ALIGN(arg_length, 4);
  memcpy(sp, arg, arg_length);
  // Update threads sp
  set_thread_sp(t, sp);
  // Set thread argument to point at the argument.
  set_thread_argument(t, (uint32_t)sp);
  // Set thread pc to function
  set_thread_pc(t, fn);

  // Thread is ready. Put in Q.
  sll_insert_back(thread_list, &t);

  if (sll_length(thread_list) > 1 && !timer_is_active(timer_handle)) {
    // This causes the current thread to get its full quantum again.
    timer_handle = timer_set(QUANTUM_IN_MILLIS, ms, schedule, true);
  } else if (sll_length(thread_list) == 1) {
    // Single Pulse. We need this because context can only be switched during an
    // Exceptions.
    timer_handle = timer_set(1, ms, schedule, false);
  }
}

void start_scheduling() {
  if (sll_length(thread_list) > 0) {
    timer_handle = timer_set(QUANTUM_IN_MILLIS, ms, schedule, true);
  }

  scheduler_started = true;
}