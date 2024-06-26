.text
.global task_yield
.global mutex_acquire
.balign 4
mutex_acquire:
    addi                sp, sp, -24
    sd                  ra, 16(sp)
    sd                  s0, 8(sp)
    sd                  a0, 0(sp)
    addi                s0, sp, 16
    //init
loop:
    li                  a5, 1
    amoswap.w.aq        a5, a5, (a0)
    beqz                a5, acquire_success 
    call                task_yield
    ld                  a0, 0(sp)
    j                   loop
acquire_success:
    //finish
    ld                  s0, 8(sp)
    ld                  ra, 16(sp)
    addi                sp, sp, 24
    ret


.global mutex_release
.balign 4
mutex_release:
    addi                sp, sp, -16
    sd                  ra, 8(sp)
    sd                  s0, 0(sp)
    addi                s0, sp, 16
    //init
    li                  a5, 0
    sw                  a5, 0(a0)
    call                task_yield
    //finish
    ld                  s0, 0(sp)
    ld                  ra, 8(sp)
    addi                sp, sp, 16
    ret


.end







