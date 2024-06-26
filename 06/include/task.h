#ifndef TASK_H
#define TASK_H

#include "types.h"
#include "platform.h"
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
	reg_t a7;
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
	uint8_t priority;
	uint8_t flags;//the 0st bit means if this context works. 0 means idle
};

int task_create(void (*start_routin)(void* ), void* param, uint8_t priority );
void task_delay(volatile int count);
void task_yield();
void sched_init(void);
void schedule(void);
void os_main(void);
void task_exit();

#endif