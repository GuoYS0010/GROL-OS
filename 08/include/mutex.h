#ifndef __MUTEX_H__
#define __MUTEX_H__
#include "platform.h"
#include "types.h"

#define MUTEX_IS_FREE_FLAG 0
#define MUTEX_IS_BUSY_FLAG 1
struct mutex{
    uint32_t flag;
    //0 means free
    //1 means busy
};

extern void mutex_acquire(struct mutex *lock);
extern void mutex_release(struct mutex *lock);
//these functions are written in mutex.S
#endif