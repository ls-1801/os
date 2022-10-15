.section .init
.global _start
_start:
	/* Erkennen ob Hypervisor Mode aktiv */
	mrs r0, cpsr
	and r0, r0, #0x1F
	mov r1, #0x1A	//Modebits = 0x1A = Hypervisor Mode
	cmp r0, r1		//Vergleichen und Statusbits setzen f�r bedingte Spr�nge
	
	/* Wenn Hypemode erkannt: abschalten (sprung nach _exitHyper)
	 * wenn nicht erkannt: dann weitermachen (weiterlaufen nach _bsprak)
	 */
	beq _exitHyper
	 
_bsprak:
	/* Einsprung ins Hauptprogramm */

	// FIQ 	 
	 CPS #0b10001
	 CPSID if // disable IRQ while in FIQ Mode
	 ldr sp, =__fiq_stack_start
	
	//ABORT
	 CPS #0b10111
	 CPSID if // disable IRQ while in ABORT Mode
	 ldr sp, =__abt_stack_start
	
	//UNDEFINED
	 CPS #0b11011
	 CPSID if // disable IRQ while in UND Mode
	 ldr sp, =__und_stack_start
	
	// IRQ
	 CPS #0b10010
	 ldr sp, =__irq_stack_start
	 
	// SVC
	 CPS #0b10011
	 CPSID if // disable IRQ while in SVC Mode
	 ldr sp, =__svc_stack_start
	
	//USER / SYS
	 CPS #0b11111
	 CPSID if // disable IRQ while startup
	 ldr sp, =__sys_stack_start
	 LDR r0, =_ivt
	 MCR p15, 0, r0, c12, c0, 0
	 
	bl      start_kernel
	
.Lend:  
		b       .Lend
_exitHyper:

	mrs r0, cpsr
	bic r0, r0, #0x1F
	orr r0, r0, #0x13	//Supervisormode
	msr spsr_hyp, r0
	eret



