#ifndef __SEMAPHORE__H
#define __SEMAPHORE__H
#include "platform.h"
#include "types.h"


struct semaphore{
    uint32_t value;
    //0 means free
    //1 means busy
    uint64_t list[10];
    uint32_t is_init;
};
//extern void mutex_acquire(struct mutex *lock);
//extern void mutex_release(struct mutex *lock);
extern void semaphore_wait(struct semaphore *sem);
extern void semaphore_signal(struct semaphore *sem);
extern void semaphore_init(struct semaphore * mysema);
//these functions are written in mutex.S
#endif