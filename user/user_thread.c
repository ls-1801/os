#include "user_thread.h"
#include "lib/io.h"
#include "lib/system.h"
#include "register-checker/regcheck.h"
#include <common.h>

#define READ_ADDR(addr) asm volatile("LDR r0, [%0]" ::"r"(addr))
#define JUMP_ADDR(addr) asm volatile("BX %0" ::"r"(addr))
#define WRITE_ADDR(addr) asm volatile("STR r0, [%0]" ::"r"(addr))

void user_thread(void *x) {
  UNUSED(x);

  char c;
  char array[1];

  for (;;) {
    // read a single char from uart.
    while (svc_gets(&c, 1) == 1) {
      switch (c) {
      case 'n':
        // load nullptr
        READ_ADDR(NULL);
        break;
      case 'p':
        // jump nullptr
        JUMP_ADDR(NULL);
        break;
      case 'd':
        // read kernel data
        READ_ADDR(0x300000);
        break;
      case 'k':
        // read kernel code
        READ_ADDR(0x100000);
        break;
      case 'K':
        // read kernel stack
        READ_ADDR(0x420069);
        break;
      case 'g':
        // read uart
        READ_ADDR(0x7E201000);
        break;
      case 'c':
        WRITE_ADDR(0x600000);
        break;
      case 's':
        // stackoverflow
        array[0x100001] = 'c';
        break;
      case 'u':
        READ_ADDR(0xC0FFEE0);
        break;
      case 'x':
        // jump stack
        JUMP_ADDR(array);
        break;
      default:
        __kprintf("Unknown Command: %c", c);
      }
    }
  }
}