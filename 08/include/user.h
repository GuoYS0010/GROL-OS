#ifndef __USER_H__
#define __USER_H__

#include "types.h"
#include "platform.h"

#include "mutex.h"



/* printf */
#include "printf.h"


#include "graphprint.h"


/* task management */
#include "task.h"

#include "riscv.h"

#define get_MTIME(reg)   asm volatile("csrr %0, time" : "=r" (reg)); 

#endif 
