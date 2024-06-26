#include "plic.h"
extern void trap_vector(void);

void trap_init()
{
	/*
	 * set the trap-vector base-address for machine-mode
	 */
	w_mtvec((reg_t)trap_vector);
}

void external_interrupt_handler()
{
	int irq = plic_claim();

	if (irq == UART0_IRQ){
		#ifdef MYPRINT
		printf("uart0 interrupt irq = %d\n\r", irq);
		#endif
      	uart_isr();
	} else if (irq) {
		#ifdef MYPRINT
		printf("unexpected interrupt irq = %d\n\r", irq);
		#endif
	}
	
	if (irq) {
		plic_complete(irq);
	}
}

reg_t trap_handler(reg_t epc, reg_t cause)
{
	reg_t return_pc = epc;
	reg_t cause_code = cause & MCAUSE_MASK_ECODE;
	#ifdef MYPRINT
	printf("TRAP! cause:%p\n\r", cause);
	#endif
	if (cause & MCAUSE_MASK_INTERRUPT) {
		/* Asynchronous trap - interrupt */
		switch (cause_code) {
		case 3:
			uart_puts("software interruption!\n\r");
			break;
		case 7:
			uart_puts("timer interruption!\n\r");
			break;
		case 11:
			#ifdef MYPRINT
			uart_puts("external interruption!\n\r");
			#endif
			external_interrupt_handler();
			break;
		default:
			printf("Unknown async exception! Code = %ld\n\r", cause_code);
			break;
		}
	} else {
		/* Synchronous trap - exception */
		printf("Sync exceptions! Code = %ld\n\r", cause_code);
		panic("OOPS! What can I do!");
		//return_pc += 4;
	}

	return return_pc;
}

void trap_test()
{
	/*
	 * Synchronous exception code = 7
	 * Store/AMO access fault
	 */
	*(int *)0x12345678 = 100;

	/*
	 * Synchronous exception code = 5
	 * Load access fault
	 */
	//int a = *(int *)0x00000000;

	uart_puts("Yeah! I'm return back from trap!\n\r");
}

