#ifndef __TIMER_H__
#define __TIMER_H__
#include "platform.h"
#include "types.h"
#include "riscv.h"
#include "printf.h"
#include "task.h"
#include "semaphore.h"

#define TIMER_INTERVAL (CLINT_TIMEBASE_FREQ / CLINT_SLICE_PER_SEC)

static uint64_t _software_interrupt_arg;
static uint64_t _software_interrupt_flag;
static struct semaphore _software_interrupt_sem;
#define SOFTWARE_NOTHING                (uint64_t)0
#define SOFTWARE_KERNEL_SWITCH          (uint64_t)1
#define SOFTWARE_TASK_CONTROLLER_FLAG   (uint64_t)2
#define SOFTWARE_USER_SWITCH_FLAG       (uint64_t)3
#define SOFTWARE_USER_KILLMYSELF        (uint64_t)4

//in flag 1, the arg means the context we want to switch

#define MAX_SWTIMER 20

#define SWTIMER_NOT_EXIST   0
#define SWTIMER_PERIODIC    1
#define SWTIMER_DISPONSIBLE 2
struct swtimer{
    void (*func)(void *arg);
    void *arg;
    uint32_t timeout_tick;
    uint32_t period;
    uint8_t flag;
    //a tick equals to (1 / CLINT_SLICE_PER_SEC) second
};


void schedule(void);
/* load timer interval(in ticks) for next timer interrupt.*/
void timer_load(int interval);

void timer_init();

void timer_handler();

void newpriority();

static uint64_t get_time();

static void get_time_str(char* time);



void software_handler(struct context *ctx);
void call_software_interrupt(uint64_t flag, uint64_t arg);
void reset_software_interrupt();





void swtimer_init();
struct swtimer *swtimer_create(
    void (*handler)(void *arg),
    void *arg,
    uint32_t timeout,
    uint8_t flag
);

void swtimer_delete(struct swtimer * swtimer);
void swtimer_check();
void swtimer_user_first_stage(
    void (*handler)(void *arg),
    void *arg);


#endif