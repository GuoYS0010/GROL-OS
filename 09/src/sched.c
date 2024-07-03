#include "task.h"

/* defined in entry.S */
extern void switch_to(struct context *next);

static uint64_t TICKS_PER_SLICE = 60;
static uint8_t _min_priority = 255;
static struct swtimer *_task_controller_timer;

/*
 * _top is used to mark the max available position of ctx_tasks
 * _current is used to point to the context of current task
 * _top is change to _num.
 */
static int _num = 0;
static int _current = -1;
void task_num_sub(){
	_num --;
}




uint8_t get_priority(){
	struct context* pmycontext = r_mscratch();
	return pmycontext->priority;
}

void newpriority(){
	_min_priority = 255;
	for (int i=0; i < MAX_TASKS; i++){
		if ((ctx_tasks[i].flags != CONTEXT_NOT_EXIST) && (ctx_tasks[i].priority < _min_priority))	_min_priority = ctx_tasks[i].priority;
	}
	#ifdef MYPRINT
	printf("new priority:%d\n\r", _min_priority);
	#endif

}
/*
 * implment a simple cycle FIFO schedular
 */
void schedule()
{

	struct context *next = &(ctx_tasks[0]);
	if (next -> flags == CONTEXT_NOT_EXIST) panic("no task controller\n\r");			//no task controller
	switch_to(next);
}

void task_exit(){
	_num--;
	call_software_interrupt(SOFTWARE_USER_KILLMYSELF, (uint64_t)&ctx_tasks[0]);
}

/*
 * DESCRIPTION
 * 	Create a task.
 * 	- start_routin: task routine entry
 * RETURN VALUE
 * 	0: success
 * 	-1: if error occured
 */
int task_create(void (*start_routin)(void* ), void* param, uint8_t priority)
{
	if (_num < MAX_TASKS) {
		int tmp = 0;
		while (ctx_tasks[tmp].flags != CONTEXT_NOT_EXIST) tmp = (tmp + 1) % MAX_TASKS;
		ctx_tasks[tmp].sp = (reg_t) &task_stack[tmp][STACK_SIZE];
		ctx_tasks[tmp].a0 = (reg_t) param;
		ctx_tasks[tmp].priority = priority;
		ctx_tasks[tmp].flags = CONTEXT_IN_LIST;
		ctx_tasks[tmp].mepc = start_routin;
		#ifdef MYPRINT
		printf("my mepc is %p \n\r", ctx_tasks[tmp].mepc);
		#endif
		_num++;
		if (_num == 1) {
			print_taskcontroller_created();
			_task_controller_timer = swtimer_create(schedule, 0, TICKS_PER_SLICE, SWTIMER_PERIODIC);
		}
		else if (priority < _min_priority) _min_priority = priority;//find the best task level
		return 0;
	} else {
		return -1;
	}
}

/*
 * DESCRIPTION
 * 	task_yield()  causes the calling task to relinquish the CPU and a new 
 * 	task gets to run.
 */


/*
 * a very rough implementaion, just to consume the cpu
 */

#define DELAY 1000
void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}
void task_delay_swtimer(volatile int count){
	
}

//kernel use this function to switch to task controller
void kernel_schedule(){
	printf("ready to switch from kernel\n\r");
	call_software_interrupt(SOFTWARE_KERNEL_SWITCH, (uint64_t)(&ctx_tasks[0]));
}

static void task_controller(void* param){
	static int next_task = 0;
	while (1){
	if (_num <= 1) {
		printf("no user task have been created\n\r");
		return;
	}else{
		printf("\033[33mI am task controller!\n\r");
		next_task  = next_task % MAX_TASKS + 1;
		printf("taskflag:%d,%d,%d,%d\n\r", ctx_tasks[0].flags,ctx_tasks[1].flags,ctx_tasks[2].flags);
		while ((ctx_tasks[next_task].flags != CONTEXT_IN_LIST) || (ctx_tasks[next_task].priority != _min_priority)) {
			next_task  = next_task % MAX_TASKS + 1;
		}
		printf("I am goint to switch to %dth task\033[0m\n\r", next_task);
		#ifdef MYPRINT
		printf("the mepc of next task is %p\n\r", ctx_tasks[next_task].mepc);
		printf("the context of next task is %p\n\r", &ctx_tasks[next_task]);
		printf("the ra of next task is %p\n\r", ctx_tasks[next_task].ra);
		#endif
		struct context * tmpcontext = &ctx_tasks[next_task];
		#ifndef MYPRINT
		printf("\033[1A");
		printf("\033[K");
		printf("\033[1A");
		printf("\033[K");
		#endif
		call_software_interrupt(SOFTWARE_TASK_CONTROLLER_FLAG, (uint64_t)tmpcontext);
	}
	}
}


void sched_init(int slice)
{
	TICKS_PER_SLICE = slice;
	w_mscratch(0);
	for (int i=0; i < MAX_TASKS; i++){
		ctx_tasks[i].flags = CONTEXT_NOT_EXIST;
	}
	task_create(task_controller, 0, 255);
	
}
