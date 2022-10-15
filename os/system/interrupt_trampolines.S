.global __current_thread_registers
.global __current_thread_spsr

.global data_abort_tr
data_abort_tr:
    SUB lr, lr, #8
    STMFD sp!, {r0-r12, lr}
    MOV r0, sp
    MRS r1, spsr
    LDR r2, =__current_thread_registers
    LDR r3, =__current_thread_spsr
    STR r0, [r2]
    STR r1, [r3]
	MOV r1, lr
    BL data_abort              //data abort handler
    LDMFD sp!, {r0-r12, pc}^

.global undefined_instruction_tr
undefined_instruction_tr:
    SUB lr, lr, #4
    STMFD sp!, {r0-r12, lr}
    MOV r0, sp
    MRS r1, spsr
    LDR r2, =__current_thread_registers
    LDR r3, =__current_thread_spsr
    STR r0, [r2]
    STR r1, [r3]
	MOV r1, lr
    BL undefined_instruction              //data abort handler
    LDMFD sp!, {r0-r12, pc}^

.global software_interrupt_tr
software_interrupt_tr:
    STMFD sp!, {r0-r12, lr}
    MOV r0, sp
    MRS r1, spsr
    LDR r2, =__current_thread_registers
    LDR r3, =__current_thread_spsr
    STR r0, [r2]
    STR r1, [r3]
    SUB lr, lr, #4      // instruction that caused the syscall,
                        // we consume the syscall
	MOV r1, lr
    BL software_interrupt              //svc handler
    LDMFD sp!, {r0-r12, pc}^

.global prefetch_abort_tr
prefetch_abort_tr:
    SUB lr, lr, #8
    STMFD sp!, {r0-r12, lr}
    MOV r0, sp
    MRS r1, spsr
    LDR r2, =__current_thread_registers
    LDR r3, =__current_thread_spsr
    STR r0, [r2]
    STR r1, [r3]
	MOV r1, lr
    BL prefetch_abort              //prefetch abort handler
    LDMFD sp!, {r0-r12, pc}^

.global fiq_tr
fiq_tr:
.global irq_tr
irq_tr:
    SUB lr, lr, #4
    STMFD sp!, {r0-r12, lr}
    MOV r0, sp
    MRS r1, spsr
    LDR r2, =__current_thread_registers
    LDR r3, =__current_thread_spsr
    STR r0, [r2]
    STR r1, [r3]
	BL irq              //irq handler
    LDMFD sp!, {r0-r12, pc}^
