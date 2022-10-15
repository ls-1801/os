#ifndef REGISTER_H
#define REGISTER_H
#include <stdint.h>

static inline uint32_t __get_r0(void) {
  int ret;
  asm volatile("MOV %0, r0" : "=r"(ret));
  return ret;
}

static inline void __set_r0(uint32_t val) {
  asm volatile("MOV r0, %0" ::"r"(val));
}

static inline uint32_t __get_r1(void) {
  int ret;
  asm volatile("MOV %0, r1" : "=r"(ret));
  return ret;
}

static inline void __set_r1(uint32_t val) {
  asm volatile("MOV r1, %0" ::"r"(val));
}

static inline uint32_t __get_r2(void) {
  int ret;
  asm volatile("MOV %0, r2" : "=r"(ret));
  return ret;
}

static inline void __set_r2(uint32_t val) {
  asm volatile("MOV r2, %0" ::"r"(val));
}

static inline uint32_t __get_r3(void) {
  int ret;
  asm volatile("MOV %0, r3" : "=r"(ret));
  return ret;
}

static inline void __set_r3(uint32_t val) {
  asm volatile("MOV r3, %0" ::"r"(val));
}

static inline uint32_t __get_r4(void) {
  int ret;
  asm volatile("MOV %0, r4" : "=r"(ret));
  return ret;
}

static inline void __set_r4(uint32_t val) {
  asm volatile("MOV r4, %0" ::"r"(val));
}

static inline uint32_t __get_r5(void) {
  int ret;
  asm volatile("MOV %0, r5" : "=r"(ret));
  return ret;
}

static inline void __set_r5(uint32_t val) {
  asm volatile("MOV r5, %0" ::"r"(val));
}
static inline uint32_t __get_r6(void) {
  int ret;
  asm volatile("MOV %0, r6" : "=r"(ret));
  return ret;
}
static inline void __set_r6(uint32_t val) {
  asm volatile("MOV r6, %0" ::"r"(val));
}
static inline uint32_t __get_r7(void) {
  int ret;
  asm volatile("MOV %0, r7" : "=r"(ret));
  return ret;
}
static inline void __set_r7(uint32_t val) {
  asm volatile("MOV r7, %0" ::"r"(val));
}
static inline uint32_t __get_r8(void) {
  int ret;
  asm volatile("MOV %0, r8" : "=r"(ret));
  return ret;
}
static inline void __set_r8(uint32_t val) {
  asm volatile("MOV r8, %0" ::"r"(val));
}
static inline uint32_t __get_r9(void) {
  int ret;
  asm volatile("MOV %0, r9" : "=r"(ret));
  return ret;
}
static inline void __set_r9(uint32_t val) {
  asm volatile("MOV r9, %0" ::"r"(val));
}
static inline uint32_t __get_r10(void) {
  int ret;
  asm volatile("MOV %0, r10" : "=r"(ret));
  return ret;
}
static inline void __set_r10(uint32_t val) {
  asm volatile("MOV r10, %0" ::"r"(val));
}
static inline uint32_t __get_r11(void) {
  int ret;
  asm volatile("MOV %0, r11" : "=r"(ret));
  return ret;
}
static inline void __set_r11(uint32_t val) {
  asm volatile("MOV r11, %0" ::"r"(val));
}
static inline uint32_t __get_r12(void) {
  int ret;
  asm volatile("MOV %0, r12" : "=r"(ret));
  return ret;
}
static inline void __set_r12(uint32_t val) {
  asm volatile("MOV r12, %0" ::"r"(val));
}
static inline uint32_t __get_sp(void) {
  int ret;
  asm volatile("MOV %0, sp" : "=r"(ret));
  return ret;
}
static inline void __set_sp(uint32_t val) {
  asm volatile("MOV sp, %0" ::"r"(val));
}

static inline uint32_t __get_lr(void) {
  int ret;
  asm volatile("MOV %0, lr" : "=r"(ret));
  return ret;
}

static inline void __set_lr(uint32_t val) {
  asm volatile("MOV lr, %0" ::"r"(val));
}

static inline uint32_t __get_pc(void) {
  uint32_t ret;
  asm volatile("MOV %0, pc" : "=r"(ret));
  return ret;
}

static inline void __set_pc(uint32_t val) {
  asm volatile("MOV pc, %0" ::"r"(val));
}

static inline uint32_t __get_cpsr(void) {
  uint32_t retval;
  asm volatile(" mrs  %0, cpsr" : "=r"(retval) : /* no inputs */);
  return retval;
}

static inline void __set_cpsr(uint32_t val) {
  asm volatile(" msr  cpsr, %0"
               : /* no outputs */
               : "r"(val));
}

static inline uint32_t __get_spsr(void) {
  uint32_t retval;
  asm volatile(" mrs  %0, spsr" : "=r"(retval) : /* no inputs */);
  return retval;
}

static inline void __set_spsr(uint32_t val) {
  asm volatile(" msr  spsr, %0"
               : /* no outputs */
               : "r"(val));
}

#endif
