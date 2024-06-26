#ifndef __RISCV_H__
#define __RISCV_H__
#include "types.h"

/* 
 *  register operation of multi-task.
 */
static void w_mscratch(reg_t x){
	asm volatile("csrw mscratch, %0" : : "r" (x));
}

static reg_t r_mscratch(){
	reg_t ret;
	asm volatile("csrr  %0, mscratch" :  "=r" (ret):);
	return ret;
}
//mscratch register is a csr register, stand for the address of the 
//   context of the current task



/* 
 *  register operation of trap and interruption
 */

static inline reg_t r_tp(){
    reg_t ret;
    asm volatile("mv %0, tp" :"=r" (ret));
    return ret;
}
//tp register stand for the index of the current hart.
//similar with r_mhartid below.but i dont know whats the difference


static inline reg_t r_mhartid(){
	reg_t x;
	asm volatile("csrr %0, mhartid" : "=r" (x) );
	return x;
}

/* Machine Status Register, mstatus */
#define MSTATUS_MPP (3 << 11)
#define MSTATUS_SPP (1 << 8)

#define MSTATUS_MPIE (1 << 7)
#define MSTATUS_SPIE (1 << 5)
#define MSTATUS_UPIE (1 << 4)

#define MSTATUS_MIE (1 << 3)
#define MSTATUS_SIE (1 << 1)
#define MSTATUS_UIE (1 << 0)

static inline reg_t r_mstatus(){
	reg_t x;
	asm volatile("csrr %0, mstatus" : "=r" (x) );
	return x;
}
static inline void w_mstatus(reg_t x){
	asm volatile("csrw mstatus, %0" : : "r" (x));
}



/* 
 * mepc in machine mode, machine expection programmer counter
 * stand for the address where we enter the interruption(pc)
 * or pc + 4
 */
static inline void w_mepc(reg_t x){
	asm volatile("csrw mepc, %0" : : "r" (x));
}

static inline reg_t r_mepc(){
	reg_t ret;
	asm volatile("csrr %0, mepc" : "=r" (ret));
	return ret;
}
//the address of the interruption function
static inline reg_t r_mtvec(){
	reg_t ret;
	asm volatile("csrr  %0, mtvec" :  "=r" (ret):);
	return ret;
}
static inline void w_mtvec(reg_t x){
	asm volatile("csrw   mtvec, %0" :  :"r" (x):);
}


/* Machine-mode Interrupt Enable */
// set mstatus first and then mie.
#define MIE_MEIE (1 << 11) // external
#define MIE_MTIE (1 << 7)  // timer
#define MIE_MSIE (1 << 3)  // software
static inline reg_t r_mie()
{
	reg_t x;
	asm volatile("csrr %0, mie" : "=r" (x) );
	return x;
}

static inline void w_mie(reg_t x)
{
	asm volatile("csrw mie, %0" : : "r" (x));
}

/* Machine-mode Cause Masks */
#define MCAUSE_MASK_INTERRUPT	(reg_t)0x8000000000000000
#define MCAUSE_MASK_ECODE	(reg_t)0x7FFFFFFF
//which interruption. for example ,uart
static inline reg_t r_mcause()
{
	reg_t x;
	asm volatile("csrr %0, mcause" : "=r" (x) );
	return x;
}

#endif