#ifndef _SYSTEM_THREAD_H_
#define _SYSTEM_THREAD_H_

#define MAX_THREADS 32
#include "register.h"
#include <common.h>

typedef void (*threadFn)(void *);

typedef struct Thread *thread_t;

void *get_thread_sp(thread_t t);
void set_thread_sp(thread_t t, void *ptr);

void set_thread_argument(thread_t t, uint32_t arg);
void set_thread_pc(thread_t t, void *pc);

char *debug_thread(thread_t, char *buf);

void initialize_thread(thread_t);

thread_t allocate_thread();
void free_thread(thread_t);

void save_thread(thread_t);
void load_thread(thread_t);

enum ThreadRegister {
  r0 = 0,
  r1 = 1,
  r2 = 2,
  r3 = 3,
  r4 = 4,
  r5 = 5,
};

void set_thread_register(thread_t t, enum ThreadRegister tr, uint32_t v);
uint32_t get_thread_register(thread_t t, enum ThreadRegister tr);

#endif /* _SYSTEM_THREAD_H_ */