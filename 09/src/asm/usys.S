.text
.global user_syscall
.balign 4
user_syscall:
    addi    a7, a0, 0
    ecall
    ret
