#define LOAD		ld
#define SIZE_REG    8
.macro reg_restore base
	LOAD ra,   0*SIZE_REG(\base)
	LOAD sp,   1*SIZE_REG(\base)
	LOAD t0,   4*SIZE_REG(\base)
	LOAD t1,   5*SIZE_REG(\base)
	LOAD t2,   6*SIZE_REG(\base)
	LOAD s0,   7*SIZE_REG(\base)
	LOAD s1,   8*SIZE_REG(\base)
	LOAD a0,   9*SIZE_REG(\base)
	LOAD a1,  10*SIZE_REG(\base)
	LOAD a2,  11*SIZE_REG(\base)
	LOAD a3,  12*SIZE_REG(\base)
	LOAD a4,  13*SIZE_REG(\base)
	LOAD a5,  14*SIZE_REG(\base)
	LOAD a6,  15*SIZE_REG(\base)
	LOAD a7,  16*SIZE_REG(\base)
	LOAD s2,  17*SIZE_REG(\base)
	LOAD s3,  18*SIZE_REG(\base)
	LOAD s4,  19*SIZE_REG(\base)
	LOAD s5,  20*SIZE_REG(\base)
	LOAD s6,  21*SIZE_REG(\base)
	LOAD s7,  22*SIZE_REG(\base)
	LOAD s8,  23*SIZE_REG(\base)
	LOAD s9,  24*SIZE_REG(\base)
	LOAD s10, 25*SIZE_REG(\base)
	LOAD s11, 26*SIZE_REG(\base)
	LOAD t3,  27*SIZE_REG(\base)
	LOAD t4,  28*SIZE_REG(\base)
	LOAD t5,  29*SIZE_REG(\base)
.endm

.text
.global swtimer_user
.global user_syscall
.global reg_restore
.global testtest
.balign 4

swtimer_user:
    addi                sp, sp, -24
    sd                  ra, 8(sp)
    sd                  s0, 0(sp)
    addi                s0, sp, 24


    addi 				a2, a0, 0
    addi                a0, a1, 0
    jalr                ra, 0(a2)


    li                  a0, 3               //SYSCALL_GET_MSCRATCH          csrr                a1, mscratch
    call                user_syscall
    ld                  t6, 31*SIZE_REG(a0)        //get mepc
    addi                t5, a0, 0
    reg_restore         t5
    
    jalr                x0, 0(t6)				//dont need t6. hope there will be no trouble


.global swtimer_user_first_stage
.balign 4
swtimer_user_first_stage:
    la                  a2, swtimer_user
    csrw                mepc, a2   
    mret

.end