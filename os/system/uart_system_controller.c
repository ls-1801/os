#include "uart_system_controller.h"
#include "scheduler.h"

#define READ_ADDR(addr) asm volatile("LDR r0, [%0]" ::"r"(addr))
#define WRITE_ADDR(addr) asm volatile("STR r0, [%0]" ::"r"(addr))
#define JUMP_ADDR(addr) asm volatile("BX %0" ::"r"(addr))

void user_thread(void *);

bool uart_system_controller(char c) {
  switch (c) {
  case 'N':
    READ_ADDR(NULL);
    break;
  case 'P':
    JUMP_ADDR(NULL);
    break;
  case 'C':
    // write kernel code
    WRITE_ADDR(0x100000);
    break;
  case 'U':
    // read unassigned
    READ_ADDR(0xC0FFEE0);
    break;
  case 'X':
    // jump user code
    JUMP_ADDR(0x600000);
    break;
  case '1':
    // Start a new thread.
    start_thread(user_thread, NULL, 0);
    return true;
  }

  return false;
}