#ifndef __TIMER_H__
#define __TIMER_H__
#include "platform.h"
#include "types.h"
#include "riscv.h"
#include "printf.h"
#include "task.h"

#define TIMER_INTERVAL (CLINT_TIMEBASE_FREQ / CLINT_SLICE_PER_SEC)

static uint64_t TICKS_PER_SLICE = 60;
static uint64_t _tick = 0;
static uint64_t _slice = 0;

static uint8_t software_interrupt_flag = 0;
#define SOFTWARE_NOTHING                0
#define SOFTWARE_KERNEL_SWITCH          1
#define SOFTWARE_TASK_CONTROLLER_FLAG   2
#define SOFTWARE_USER_SWITCH_FLAG       3
#define SOFTWARE_USER_KILLMYSELF        4

static uint64_t software_interrupt_arg;
//in flag 1, the arg means the context we want to switch



void schedule(void);
/* load timer interval(in ticks) for next timer interrupt.*/
void timer_load(int interval);

void timer_init();

void timer_handler();

void newpriority();

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



void software_handler();
void call_software_interrupt(uint8_t flag, uint64_t arg);
void reset_software_interrupt();

#endif