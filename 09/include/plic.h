#ifndef __PLIC_H__
#define __PLIC_H__
#include "platform.h"
#include "riscv.h"
#include "timer.h"
#include "syscall.h"
void trap_init();
void plic_init();
int plic_claim(void);
void plic_complete(int irq);
void task_num_sub();
#endif