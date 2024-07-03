.text
.global fifo_pop
.global user_syscall

.global semaphore_wait
.balign 4
semaphore_wait:
    addi                sp, sp, -40
    sd                  ra, 8(sp)
    sd                  s0, 0(sp)
    addi                s0, sp, 40
    //init
    li                  a1, -1
    amoadd.d.aq         a2, a1, (a0)   
    bgt                 a2, x0, wait_exit 

    addi                a0, a0, 8
    sd                  a0, 16(sp)
    li                  a0, 3               //SYSCALL_GET_MSCRATCH          csrr                a1, mscratch
    call                user_syscall
    addi                a1, a0, 0
    ld                  a0, 16(sp)
    ld                  a2, (a0)
loop1:
    beq                 a2, x0, fin_loop
    addi                a0, a0, 8
    ld                  a2, (a0)
    j                   loop1
fin_loop:
    sd                  a1, (a0)            // add this process to S->list
    li                  a0, 2
    sd                  a0 , 264(a1)         //32*8+1 (uint64_t)&ctx_tasks[0].flags - (uint64_t)&ctx_tasks[0])
                                            //set context->flag = CONTEXT_OUTOF_LIST
    li                  a0, 2               //SYSCALL_JUST_YIELD
    call                user_syscall
wait_exit:
    ld                  s0, 0(sp)
    ld                  ra, 8(sp)
    addi                sp, sp, 40
    ret


.global semaphore_signal
.balign 4
semaphore_signal:
    addi                sp, sp, -24
    sd                  ra, 8(sp)
    sd                  s0, 0(sp)
    addi                s0, sp, 24
    //init

    li                  a1, 1
    amoadd.d.rl         a2, a1, (a0)
    blt                 a2, x0, remove_process
    j                   signal_exit
remove_process:
    addi                a0, a0, 8
    ld                  a1, (a0)
    sd                  a1, 16(sp)
    call                fifo_pop    //remove a process from list
    ld                  a1, 16(sp)
    li                  a2, 1
    sd                  a2 , 264(a1)         //33*8+ (uint64_t)&ctx_tasks[0].flags - (uint64_t)&ctx_tasks[0])
                                            //set context->flag = CONTEXT_IN_LIST   wakeup

 
signal_exit:
    ld                  s0, 0(sp)
    ld                  ra, 8(sp)
    addi                sp, sp, 24
    ret


.global semaphore_init
.balign 4
semaphore_init:
    addi                sp, sp, -16
    sd                  ra, 8(sp)
    sd                  s0, 0(sp)
    addi                s0, sp, 16
    //init
    li                  a1, 1
    addi                a2, a0, 88
    amoswap.w.aq        a3, a1, (a2)
    beq                 a3, x0, myinit
    j                   init_exit
    myinit:
    sd                  a1, (a0)
    addi                a0, a0, 4
    sd                  x0, (a0)
    addi                a0, a0, 8
    sd                  x0, (a0)
    addi                a0, a0, 8
    sd                  x0, (a0)
    addi                a0, a0, 8
    sd                  x0, (a0)
    addi                a0, a0, 8
    sd                  x0, (a0)
    addi                a0, a0, 8
    sd                  x0, (a0)
    addi                a0, a0, 8
    sd                  x0, (a0)
    addi                a0, a0, 8
    sd                  x0, (a0)
    addi                a0, a0, 8
    sd                  x0, (a0)
    addi                a0, a0, 8
    sd                  x0, (a0)
    init_exit:
    ld                  s0, 0(sp)
    ld                  ra, 8(sp)
    addi                sp, sp, 16
    ret



.end








