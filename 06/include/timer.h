#ifndef __TIMER_H__
#define __TIMER_H__
#include "platform.h"
#include "types.h"
#include "riscv.h"
#include "printf.h"

#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ

static uint64_t _tick = 0;
/* load timer interval(in ticks) for next timer interrupt.*/
void timer_load(int interval);

void timer_init();

void timer_handler();

uint64_t get_time(){
    return _tick;
}

void get_time_str(char* time){
    int index = 0;
    uint8_t tmp = (_tick / 3600);
    time[0] = (tmp % 100)/ 10 + '0'; 
    time[1] = tmp % 10 + '0'; 
    time[2] = ':';
    tmp = (_tick % 3600)/60;
    time[3] = tmp / 10 + '0'; 
    time[4] = tmp % 10 + '0'; 
    time[5] = ':';
    tmp = _tick % 60;
    time[6] = tmp / 10 + '0'; 
    time[7] = tmp % 10 + '0'; 

}


#endif