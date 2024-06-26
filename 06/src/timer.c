#include "timer.h"

/* load timer interval(in ticks) for next timer interrupt.*/
void timer_load(int interval)
{
	/* each CPU has a separate source of timer interrupts. */
	int id = r_mhartid();
    reg_t MTIME;
    get_MTIME(MTIME);
	MTIME += interval;
	*(uint32_t*)CLINT_MTIMECMPL(id) = (uint32_t)(MTIME & 0xffffffff) ;
	*(uint32_t*)CLINT_MTIMECMPH(id) = (uint32_t)((MTIME >> 32) & 0xffffffff) ;
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
    #ifdef MYPRINT
	printf("tick: %d\n\r", _tick);
    #endif

	timer_load(TIMER_INTERVAL);
	char timestr[9];
	get_time_str(timestr);
	timestr[8] = 0;
    #ifdef MYPRINT
	printf("%s\n\r", timestr);
    #endif
}
