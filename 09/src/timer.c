#include "timer.h"
extern void switch_to(struct context *next);

static uint64_t _tick = 0;
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

	reg_t hartid = r_mhartid();
	w_msip(hartid, 0);//disable software interrupt
	/* enable machine-mode global interrupts. */
	w_mstatus(r_mstatus() | MSTATUS_MIE);
	semaphore_init(&_software_interrupt_sem);
	printf("timer init!!\n\r");
}


static uint64_t get_time(){
    return _tick / CLINT_SLICE_PER_SEC;
}

static void get_time_str(char* time){
    int index = 0;
    uint64_t tick_sec = _tick / CLINT_SLICE_PER_SEC;
    uint8_t tmp = (tick_sec / 3600);
    time[0] = (tmp % 100)/ 10 + '0'; 
    time[1] = tmp % 10 + '0'; 
    time[2] = ':';
    tmp = (tick_sec % 3600)/60;
    time[3] = tmp / 10 + '0'; 
    time[4] = tmp % 10 + '0'; 
    time[5] = ':';
    tmp = tick_sec % 60;
    time[6] = tmp / 10 + '0'; 
    time[7] = tmp % 10 + '0'; 

}

void timer_handler() 
{
	_tick++;
    #ifdef MYPRINT
	//printf("tick: %d\n\r", _tick);
    #endif

	timer_load(TIMER_INTERVAL);

	char timestr[9];
	get_time_str(timestr);
	timestr[8] = 0;
	swtimer_check();
}


void software_handler(struct context *ctx){
	uint8_t flag = _software_interrupt_flag;
	uint64_t arg = _software_interrupt_arg;
	

	semaphore_signal(&_software_interrupt_sem);
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
			w_mstatus_MPP(MSTATUS_MPP_MACHINE);
			schedule();
			break;
		case SOFTWARE_TASK_CONTROLLER_FLAG:	
			w_mstatus_MPP(MSTATUS_MPP_USER);
			#ifdef MYPRINT
			printf("task controller calling\n\r");
			#endif		
			switch_to((uint64_t) arg);
			break;
		case SOFTWARE_USER_SWITCH_FLAG:
			printf("user want to switch\n\r");
			w_mstatus_MPP(MSTATUS_MPP_MACHINE);
			schedule();
			break;
		case SOFTWARE_USER_KILLMYSELF:
			pmycontext->flags = 0;
			newpriority();
			w_mstatus_MPP(MSTATUS_MPP_MACHINE);
			schedule();
			break;
		default:
			panic("UNKNOWN SOFTWARE INTERRUPT!!\n\r");
			break;
	}
}



void call_software_interrupt(uint64_t flag, uint64_t arg){
	semaphore_wait(&_software_interrupt_sem);
	_software_interrupt_flag = flag;
	_software_interrupt_arg = arg;
	reg_t hartid = r_mhartid();
	w_msip(hartid, 1);
}
void reset_software_interrupt(){
	_software_interrupt_flag = 0;
	_software_interrupt_arg = 0;
	reg_t hartid = r_mhartid();
	w_msip(hartid, 0);
}

void testtest(uint64_t a, uint64_t b){
	printf("testtest###%p, %p\n\r",a,b);
}

static struct swtimer _swtimer[MAX_SWTIMER];
static struct semaphore swtimer_sem;
void swtimer_init(){
    for (int i=0; i < MAX_SWTIMER; i++){
        _swtimer[i].flag = SWTIMER_NOT_EXIST;
    }
	semaphore_init(&swtimer_sem);
}

struct swtimer *swtimer_create(
    void (*handler)(void *arg),
    void *arg,
    uint32_t timeout,
    uint8_t flag
){
    int avalible_timer;
    semaphore_wait(&swtimer_sem);
    for (avalible_timer=0; avalible_timer < MAX_SWTIMER; avalible_timer++)
        if (SWTIMER_NOT_EXIST == _swtimer[avalible_timer].flag) break;
    _swtimer[avalible_timer].func = handler;
    _swtimer[avalible_timer].arg = arg;
    _swtimer[avalible_timer].timeout_tick = timeout + _tick;
    _swtimer[avalible_timer].flag = flag;
	_swtimer[avalible_timer].period = timeout;
    semaphore_signal(&swtimer_sem);
	#ifndef myprint
	printf("swtimer created.id=%d, current_tick=%d, next_tick=%d\n\r", avalible_timer, _tick, _swtimer[avalible_timer].timeout_tick);
	#endif
}

void swtimer_delete(struct swtimer * swtimer){

    semaphore_wait(&swtimer_sem);
    swtimer -> flag = SWTIMER_NOT_EXIST;
    semaphore_signal(&swtimer_sem);
}

void swtimer_check(){
	for (int i=0; i < MAX_SWTIMER; i++){
		if ((_swtimer[i].timeout_tick <= _tick) && (_swtimer[i].flag != SWTIMER_NOT_EXIST)){
			#ifdef MYPRINT
			printf("swtimer:id=0,current_tick=%d, timeouttick=%d\n\r",  _tick, _swtimer[0].timeout_tick);
			printf("mepc=%p\n\r", r_mepc());
			#endif
			if (_swtimer[i].flag == SWTIMER_DISPONSIBLE) _swtimer[i].flag = SWTIMER_NOT_EXIST;
			_swtimer[i].timeout_tick = _swtimer[i].period + _tick;
			if (_swtimer[i].func == schedule){
				#ifdef MYPRINT
				printf("swtimer call machine func!!!\n\r");
				#endif
				w_mstatus_MPP(MSTATUS_MPP_MACHINE);
				_swtimer[i].func(_swtimer[i].arg);
			}
			else{
				#ifdef MYPRINT
				printf("swtimer call user func!!!\n\r");
				#endif
				w_mstatus_MPP(MSTATUS_MPP_USER);
				swtimer_user_first_stage(_swtimer[i].func, _swtimer[i].arg);

			}
		}
	}
}