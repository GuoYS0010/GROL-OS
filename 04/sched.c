#include "os.h"
#include "graphprint/graphprint.h"

/* defined in entry.S */
extern void switch_to(struct context *next);

#define MAX_TASKS 10
#define STACK_SIZE 1024
/*
 * In the standard RISC-V calling convention, the stack pointer sp
 * is always 16-byte aligned.
 */
uint8_t __attribute__((aligned(16))) task_stack[MAX_TASKS][STACK_SIZE];
struct context ctx_tasks[MAX_TASKS];

/*
 * _top is used to mark the max available position of ctx_tasks
 * _current is used to point to the context of current task
 * _top is change to _num.
 */
static int _num = 0;
static int _current = -1;

static void w_mscratch(reg_t x)
{
	asm volatile("csrw mscratch, %0" : : "r" (x));
}
static reg_t r_mscratch()
{
	reg_t ret;
	asm volatile("csrr  %0, mscratch" :  "=r" (ret):);
	return ret;
}
reg_t r_mtvec()
{
	reg_t ret;
	asm volatile("csrr  %0, mtvec" :  "=r" (ret):);
	return ret;
}
reg_t r_mstatus()
{
	reg_t ret;
	asm volatile("csrr  %0, mstatus" :  "=r" (ret):);
	return ret;
}

uint8_t get_priority(){
	struct context* pmycontext = r_mscratch();
	return pmycontext->priority;
}

/*
 * implment a simple cycle FIFO schedular
 */
void schedule()
{
	struct context *next = &(ctx_tasks[0]);
	switch_to(next);
}

void task_exit(){
	struct context* pmycontext = r_mscratch();
	pmycontext->flags = 0;
	_num--;
	schedule();
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
		while (ctx_tasks[tmp].flags == 1) tmp = (tmp + 1) % MAX_TASKS;
		ctx_tasks[tmp].sp = (reg_t) &task_stack[tmp][STACK_SIZE];
		ctx_tasks[tmp].ra = (reg_t) start_routin;
		ctx_tasks[tmp].a0 = (reg_t) param;
		ctx_tasks[tmp].priority = priority;
		ctx_tasks[tmp].flags = 1;
		_num++;
		if (_num == 1) print_taskcontroller_created();
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
void task_yield()
{
	schedule();
}

/*
 * a very rough implementaion, just to consume the cpu
 */

#define DELAY 1000
void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}

static void task_controller(void* param){
	static int next_task = 0;
	if (_num <= 1) {
		panic("Num of task should be greater than zero!");
		return;
	}
	while (1){
		printf("I am task controller!\n\r");
		task_delay(DELAY);
		next_task  = next_task % MAX_TASKS + 1;
		while (ctx_tasks[next_task].flags == 0) next_task  = next_task % MAX_TASKS + 1;
		printf("I am goint to switch to %dth task\n\r", next_task);
		struct context * tmpcontext = &ctx_tasks[next_task];
		switch_to(tmpcontext);
	}
}


void sched_init()
{
	w_mscratch(0);
	for (int i=0; i < MAX_TASKS; i++){
		ctx_tasks[i].flags = 0;
	}
	task_create(task_controller, 0, 0);

}