#include "interrupts.h"
#include "../lib/bits.h"
#include "../lib/io.h"
#include "debugutil.h"
#include "register.h"
#include "thread.h"

#define IR_BASE (0x7E00B000 + 0x200)

#define IRQ_BASIC_PENDING 0
#define IRQ_1_PENDING 1
#define IRQ_2_PENDING 2
#define IRQ_1 0
#define IRQ_2 1
#define IRQ_BASIC 2

static bool irq_debug_mode = false;

void irq_enable_debug_mode(bool en) { irq_debug_mode = en; }

typedef struct InterruptRegister {
  int32_t irq_pending[3];
  int32_t fiq_control;
  int32_t enable_irq[3];
  int32_t disable_irq[3];
} InterruptRegister;

static volatile InterruptRegister *ir = (InterruptRegister *)IR_BASE;

static handler_t handlers[32 * 3];

static uint32_t irqs[3];

static inline void handle_irq(int32_t handler_pos);
static void dispatch_irq();

void set_irq_interrupt_handler(enum PeripheralInterrupts pi,
                               handler_t handler) {
  if (pi < 64) {
    handlers[pi + 32] = handler; // PERIPH
  } else {
    handlers[pi - 64] = handler; // BASIC IRQ
  }
}

void enable_irq(enum PeripheralInterrupts pi) {
  if (pi < 32) {
    SET_BIT(irqs[IRQ_1_PENDING], pi);
    SET_BIT(ir->enable_irq[IRQ_1], pi);
  } else if (pi < 64) {
    SET_BIT(irqs[IRQ_2_PENDING], (pi - 32));
    SET_BIT(ir->enable_irq[IRQ_2], (pi - 32));
  } else {
    SET_BIT(irqs[IRQ_BASIC_PENDING], (pi - 64));
    SET_BIT(ir->enable_irq[IRQ_BASIC], (pi - 64));
  }
}

void disable_irq(enum PeripheralInterrupts pi) {
  if (pi < 32) {
    UNSET_BIT(irqs[IRQ_1_PENDING], pi);
    SET_BIT(ir->disable_irq[IRQ_1], pi);
  } else if (pi < 64) {
    UNSET_BIT(irqs[IRQ_2_PENDING], (pi - 32));
    SET_BIT(ir->disable_irq[IRQ_2], (pi - 32));
  } else {
    UNSET_BIT(irqs[IRQ_BASIC_PENDING], (pi - 64));
    SET_BIT(ir->disable_irq[IRQ_BASIC], (pi - 64));
  }
}

static void dispatch_irq() {
  // FIND THE INTERRUPT THAT IS PENDING
  int32_t i;
  for (i = 0; i < 3; i++) {
    int32_t en_irqs = irqs[i];
    while (en_irqs) {
      /* POSITION OF MS SET BIT */
      int32_t pos = 31 - FFS(en_irqs);
      UNSET_BIT(en_irqs, pos);

      /* CHECK IF INTERRUPT IS PENDING */
      if (CHECK_BIT(ir->irq_pending[i], pos))
        /* HANDLE IRQ */
        handle_irq((32 * i) + pos);
    }
  }
}

static inline void handle_irq(int32_t handler_pos) {
  handler_t handler = handlers[handler_pos];
  if (handler)
    handler();
}

void irq(uint32_t *regs, uint32_t spsr) {
  uint32_t cpsr = __get_cpsr();

  if (irq_debug_mode) {
    print_bar();
    kprintf("IRQ Debug Message!\n");
    kprintf("\n>>> Register snapshot (current mode) <<<\n");

    // SP and PC are put on stack after STMFD {r0-r12, lr} as a result we need
    // to shift all registers up by two and access pc at 0 an sp at 1.
    kprintf("R0:\t%x\tR8:\t%x\n", regs[2 + R0], regs[R8 + 2]);
    kprintf("R1:\t%x\tR9:\t%x\n", regs[2 + R1], regs[R9 + 2]);
    kprintf("R2:\t%x\tR10:\t%x\n", regs[2 + R2], regs[R10 + 2]);
    kprintf("R3:\t%x\tR11:\t%x\n", regs[2 + R3], regs[R11 + 2]);
    kprintf("R4:\t%x\tR12:\t%x\n", regs[2 + R4], regs[R12 + 2]);
    kprintf("R5:\t%x\tSP:\t%x\n", regs[2 + R5], regs[1]);
    kprintf("R6:\t%x\tLR:\t%x\n", regs[2 + R6], regs[13 + 2]);
    kprintf("R7:\t%x\tPC:\t%x\n", regs[2 + R7], regs[0]);

    status_register_snapshot(cpsr, spsr, irq_mode);
  }

  dispatch_irq();
}