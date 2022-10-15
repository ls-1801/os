ENTRY(_start)
SECTIONS
{
	. = 0x008000;
	.init : { *(.init) }
	
	. = 0x100000;
	.kernel_text : {
		os/*.o(.text*)
	}
	
	. = 0x200000;
	.kernel_rodata : {
		os/*.o(.rodata*)
	}
	
	. = 0x300000;
	.kernel_data_bss : {
		os/*.o(.data*)
		os/*.o(.bss*)
		os/*.o(COMMON*)
	}
	/*
	Stacks Growing Downwards
	0x500000 |svc_stack_start
	 		 |svc_stack_end / fiq_stack_start
             |fiq_stack_end / irq_stack_start
             |irq_stack_end / abt_stack_start
             |abt_stack_end / und_stack_start
             |und_stack_end / sys_stack_start
	0x400000 |sys_stack_end

	Choosing 0x4FFFF8 rather than 50000 because it wouldn't be on the same page.
    */
	. = 0x4FFFF8;
	.kernel_stack : {		
    	. = ALIGN(8);
		__fiq_stack_size = 16k;
		__und_stack_size = 16k;
		__abt_stack_size = 16k;
		__irq_stack_size = 16k;
		__svc_stack_size = 64k;
		__sys_stack_size = 1m - 8 - __fiq_stack_size - __und_stack_size - __abt_stack_size - __irq_stack_size - __svc_stack_size;
		
		
		__svc_stack_start = .;
		__fiq_stack_start = __svc_stack_start - __svc_stack_size;
		__irq_stack_start = __fiq_stack_start - __fiq_stack_size;
		__abt_stack_start = __irq_stack_start - __irq_stack_size;
		__und_stack_start = __abt_stack_start - __abt_stack_size;
		__sys_stack_start = __und_stack_start - __und_stack_size;
	}

	. = 0x500000;
	.user_stack : {
		. = ALIGN(8);
		__user_stack_end = .;
		__user_stack_start = . + 1m - 1;
	}

	. = 0x600000;
	.user_text : {
		user/*.o(.text*)
	}

	. = 0x700000;
	.user_rodata : {
		user/*.o(.rodata*)
	}
}

