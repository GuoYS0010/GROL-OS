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

	printf("Yeah! I'm return back from trap!\n\r");
}
static void syscall_handler(struct context * syscall_context){
	printf("syscall_code:%d\n\r", syscall_context -> a7);
	switch (syscall_context -> a7) {
	case SYSCALL_KILL_MYSELF :
		#ifdef MYPRINT
		printf("ask for kill itself\n\r");
		#endif
		task_num_sub();
		syscall_context -> flags = 0;
		newpriority();
		w_mstatus_MPP(MSTATUS_MPP_MACHINE);
		schedule();		
		break;
	case SYSCALL_GET_MTIME:
		#ifdef MYPRINT
		printf("ask for mtime\n\r");
		#endif
		syscall_context -> a0 = r_time();
		break;
	case SYSCALL_JUST_YIELD:
		#ifdef MYPRINT
		printf("ask for yield\n\r");
		#endif
		
		w_mstatus_MPP(MSTATUS_MPP_MACHINE);
		schedule();		
		break;
	case SYSCALL_GET_MSCRATCH:
		#ifdef MYPRINT
		printf("ask for mscratch:%p\n\r", r_mscratch());
		#endif
		syscall_context -> a0 = r_mscratch();
		break;
	default:
		panic("UNKNOWN SYSCALL_CODE\n\r");
		break;
	}
}
reg_t trap_handler(reg_t epc, reg_t cause, struct context *ctx)
{
	reg_t return_pc = epc;
	reg_t cause_code = cause & MCAUSE_MASK_ECODE;
	// #ifdef MYPRINT
	// printf("TRAP! cause:%p\n\r", cause);
	// #endif
	if (cause & MCAUSE_MASK_INTERRUPT) {
		/* Asynchronous trap - interrupt */
		switch (cause_code) {
		case 3:
			#ifdef MYPRINT
			printf("software interruption!\n\r");
			printf("mtval = %p\n\r", r_mtval());
			printf("mepc = %p\n\r", epc);
			//printf("a7a6%d,%p,%d,%p\n\r", ctx->a7, ctx->a6);
			#endif
			software_handler(ctx);
			break;
		case 7:
			timer_handler();
			break;
		case 11:
			#ifdef MYPRINT
			printf("external interruption!\n\r");
			#endif
			external_interrupt_handler();
			break;
		default:
			printf("Unknown async exception! Code = %ld\n\r", cause_code);
			break;
		}
	} else {
		switch (cause_code){
			case 8://user sys_call
			return_pc += 4;
			ctx -> mepc += 4;
			syscall_handler(ctx);
			break;
			default:
			/* Synchronous trap - exception */
			printf("Sync exceptions! Code = %ld\n\r", cause_code);
			printf("mtval = %p\n\r", r_mtval());
			printf("mepc = %p\n\r", epc);
			panic("OOPS! What can I do!");
			break;
		}
	}

	return return_pc;
}
