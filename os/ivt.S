.balign 32
.global _ivt
_ivt:
	b           _start  
    b           undefined_instruction_tr
    b           software_interrupt_tr
    b           prefetch_abort_tr  
    b           data_abort_tr  
    b           _start  
    b           irq_tr  
    b           fiq_tr