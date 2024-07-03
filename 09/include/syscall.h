#ifndef __SYSCALL_H__
#define __SYSCALL_H__
#include "types.h"
#define SYSCALL_KILL_MYSELF         0
#define SYSCALL_GET_MTIME           1
#define SYSCALL_JUST_YIELD          2
#define SYSCALL_GET_MSCRATCH        3
#define SYSCALL_SEMAPHORE_WAIT      4
#define SYSCALL_SEMAPHORE_SIGNAL    5
extern uint64_t user_syscall(uint64_t syscall_type);
#endif