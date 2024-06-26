#include "timer.h"
extern void switch_to(struct context *next);

/* load timer interval(in ticks) for next timer interrupt.*/
void timer_load(int interval)
{
	/* each CPU has a separate source of timer interrupts. */
	int id = r_mhartid();
    reg_t MTIME = r_time();
	w_mtimecmp(MTIME + interval);
	}

void timer_init()
{
	/*
	 * On reset, mtime is cleared to zero, but the mtimecmp registers 
	 * are not reset. So we have to init the mtimecmp manually.
	 */
	timer_load(TIMER_INTERVAL);

	/* enable machine-mode timer interrupts. */
	w_mie(r_mie() | MIE_MTIE);

	/* enable machine-mode global interrupts. */
	w_mstatus(r_mstatus() | MSTATUS_MIE);
	printf("timer init!!\n\r");
}

void timer_handler() 
{
	_tick++;
	_slice++;
    // #ifdef MYPRINT
	// printf("tick: %d\n\r", _tick);
    // #endif

	timer_load(TIMER_INTERVAL);
	char timestr[9];
	get_time_str(timestr);
	timestr[8] = 0;
	if (_slice >= TICKS_PER_SLICE){
		schedule();
	}
}


void software_handler(){
	uint8_t flag = software_interrupt_flag;
	uint64_t arg = software_interrupt_arg;
	reset_software_interrupt();
	struct context* pmycontext = r_mscratch();
	switch(flag){
		case SOFTWARE_NOTHING:
			printf("nothing happend\n\r");
			break;
		case SOFTWARE_KERNEL_SWITCH:
			#ifdef MYPRINT
			printf("kernel switch to task controller\n\r");
			printf("arg%p\n\r", arg);
			#endif
			schedule();
			break;
		case SOFTWARE_TASK_CONTROLLER_FLAG:
			//printf("task controller calling\n\r");
			_slice = 0;
			switch_to((uint64_t) arg);
			break;
		case SOFTWARE_USER_SWITCH_FLAG:
			printf("user want to switch\n\r");
			schedule();
			break;
		case SOFTWARE_USER_KILLMYSELF:
			pmycontext->flags = 0;
			newpriority();
			schedule();
			break;
		default:
			printf("UNKNOWN SOFTWARE INTERRUPT!!\n\r");
			break;
	}
}


void call_software_interrupt(uint8_t flag, uint64_t arg){
	software_interrupt_flag = flag;
	software_interrupt_arg = arg;
	reg_t hartid = r_mhartid();
	w_msip(hartid, 1);
}
void reset_software_interrupt(){
	software_interrupt_flag = 0;
	software_interrupt_arg = 0;
	reg_t hartid = r_mhartid();
	w_msip(hartid, 0);
}
