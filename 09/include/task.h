#ifndef TASK_H
#define TASK_H

#include "types.h"
#include "platform.h"
#include "timer.h"

#define MAX_TASKS 10
#define STACK_SIZE 1024


struct context {
	/* ignore x0 */
	reg_t ra;
	reg_t sp;
	reg_t gp;
	reg_t tp;
	reg_t t0;
	reg_t t1;
	reg_t t2;
	reg_t s0;
	reg_t s1;
	reg_t a0;
	reg_t a1;
	reg_t a2;
	reg_t a3;
	reg_t a4;
	reg_t a5;
	reg_t a6;
	reg_t a7;    //0x10
	reg_t s2;
	reg_t s3;
	reg_t s4;
	reg_t s5;
	reg_t s6;
	reg_t s7;
	reg_t s8;
	reg_t s9;
	reg_t s10;
	reg_t s11;
	reg_t t3;
	reg_t t4;
	reg_t t5;
	reg_t t6;
	reg_t mepc; //0x1f
	reg_t priority;
	reg_t flags;//the 0st bit means if this context works. 0 means not exist
};
#define CONTEXT_NOT_EXIST 	0
#define CONTEXT_IN_LIST		1
#define CONTEXT_OUTOF_LIST	2
/*
 * In the standard RISC-V calling convention, the stack pointer sp
 * is always 16-byte aligned.
 */
uint8_t __attribute__((aligned(16))) task_stack[MAX_TASKS][STACK_SIZE];
struct context ctx_tasks[MAX_TASKS];

int task_create(void (*start_routin)(void* ), void* param, uint8_t priority );
void task_delay(volatile int count);
void sched_init(int slice );
void os_main(void);	
//switch from usertask to taskcontroller
void task_exit();
//exit a usertask itself
void kernel_schedule();
//switch from kernel to taskcontroller

#endif