- [Glorious RISC-V On LicheePi - OS (GROL-OS)](#glorious-risc-v-on-licheepi---os-grol-os)
  - [-1. 勘误](#-1-勘误)
  - [0. 前言](#0-前言)
    - [进度概览](#进度概览)
    - [前置知识](#前置知识)
    - [这个工程要做的](#这个工程要做的)
    - [这个工程会让人学到的](#这个工程会让人学到的)
  - [1.前置知识补充](#1前置知识补充)
    - [C语言调用某一条汇编指令](#c语言调用某一条汇编指令)
    - [什么是栈](#什么是栈)
    - [汇编前置知识进阶](#汇编前置知识进阶)
      - [directive指令](#directive指令)
        - [`.global`](#global)
        - [`.align`](#align)
        - [写数据相关](#写数据相关)
        - [`.equ`](#equ)
      - [RISC-V的通用寄存器](#risc-v的通用寄存器)
      - [RISCV 的控制状态寄存器（CSR）](#riscv-的控制状态寄存器csr)
      - [伪指令](#伪指令)
        - [`li`](#li)
        - [`la`](#la)
    - [操作系统前置知识](#操作系统前置知识)
      - [一个非常经典的问题：CPU上电后究竟干了什么](#一个非常经典的问题cpu上电后究竟干了什么)
    - [环境配置](#环境配置)
  - [2. Hello GROL-OS](#2-hello-grol-os)
    - [框架](#框架)
    - [UART操作-以qemu+ns16550a为例](#uart操作-以qemuns16550a为例)
      - [初始化UART](#初始化uart)
      - [UART输出字符](#uart输出字符)
    - [Makefile 究竟是干什么的](#makefile-究竟是干什么的)
    - [qemu仿真](#qemu仿真)
    - [上板运行](#上板运行)
      - [修改UART的寄存器offset](#修改uart的寄存器offset)
      - [修改UART的基地址](#修改uart的基地址)
      - [链接串口线](#链接串口线)
      - [启动uboot](#启动uboot)
      - [如何在uboot中启动内核](#如何在uboot中启动内核)
      - [最后的小问题](#最后的小问题)
  - [3. printf \&\& page alloc](#3-printf--page-alloc)
    - [C语言中变量与内存的关系](#c语言中变量与内存的关系)
    - [连接器脚本的实现](#连接器脚本的实现)
      - [链接器脚本的作用](#链接器脚本的作用)
      - [链接器脚本代码解析](#链接器脚本代码解析)
      - [链接器脚本中的变量调用](#链接器脚本中的变量调用)
    - [printf的实现](#printf的实现)
    - [在堆中实现页(4K)的分配](#在堆中实现页4k的分配)
    - [修改Makefile](#修改makefile)
  - [4. 协作式多任务](#4-协作式多任务)
    - [多任务](#多任务)
    - [上下文 \&\& mscratch 寄存器](#上下文--mscratch-寄存器)
    - [一些娱乐](#一些娱乐)
  - [5. 中断和异常](#5-中断和异常)
    - [异常控制流](#异常控制流)
      - [轮询](#轮询)
      - [中断是什么](#中断是什么)
      - [异常处理](#异常处理)
      - [陷阱（trap）](#陷阱trap)
    - [陷阱的硬件实现](#陷阱的硬件实现)
      - [mtvec  (machine trap-vector base-address)](#mtvec--machine-trap-vector-base-address)
      - [mepc  (machine exception program counter)](#mepc--machine-exception-program-counter)
      - [mcause (machine cause)](#mcause-machine-cause)
      - [mtval(machine trap value)](#mtvalmachine-trap-value)
      - [mstatus(machine status)](#mstatusmachine-status)
      - [mie(machine interrupt enable)](#miemachine-interrupt-enable)
    - [陷阱的整体流程](#陷阱的整体流程)
      - [初始化](#初始化)
      - [*执行完某条指令之后要触发陷阱了*](#执行完某条指令之后要触发陷阱了)
      - [top half **（这些事都是硬件作的我们不用操心）**](#top-half-这些事都是硬件作的我们不用操心)
      - [bottom half **（这些事都是我们写软件内核要做的）**](#bottom-half-这些事都是我们写软件内核要做的)
      - [异常处理的实现](#异常处理的实现)
    - [外部中断的硬件实现-PLIC](#外部中断的硬件实现-plic)
      - [PLIC base\_address](#plic-base_address)
      - [外部中断ID](#外部中断id)
      - [PLIC register](#plic-register)
    - [外部中断的代码细节](#外部中断的代码细节)
    - [一些杂活](#一些杂活)
      - [重构项目](#重构项目)
      - [让串口输出变得好看](#让串口输出变得好看)
  - [6. 计时器中断与软件中断](#6-计时器中断与软件中断)
    - [这两种中断的硬件实现(CLINT)](#这两种中断的硬件实现clint)
      - [CLINT base\_address](#clint-base_address)
      - [CLINT register](#clint-register)
    - [抢占式多任务的流程](#抢占式多任务的流程)
    - [额外工作](#额外工作)
    - [关于c中的static关键字](#关于c中的static关键字)
  - [7.任务同步与锁](#7任务同步与锁)
    - [并发](#并发)
    - [锁](#锁)
  - [8.软件定时器与系统调用](#8软件定时器与系统调用)
    - [软件定时器和硬件定时器](#软件定时器和硬件定时器)
    - [软件定时器的代码实现](#软件定时器的代码实现)
    - [系统调用](#系统调用)
      - [权限问题](#权限问题)
      - [汇编中系统调用的代码实现](#汇编中系统调用的代码实现)
      - [通过系统调用实现一些新的功能](#通过系统调用实现一些新的功能)
        - [软件定时器的安全性问题（未完成）](#软件定时器的安全性问题未完成)
        - [锁的优化](#锁的优化)

# Glorious RISC-V On LicheePi - OS (GROL-OS)

## -1. 勘误
写的时候有很多概念本身理解也有问题，写错了好多。毕竟我也刚学。欢迎大家在issue中批评指正。我会把错误的地方都放到[errata.md](/errata.md)中。

## 0. 前言

这个项目记录了我学习操作系统的过程。记录的内容以硬件实现为主，内核的软件实现我没有写的很详细，如果有疑问可以移步[b站汪辰老师的慕课](https://www.bilibili.com/video/BV1Q5411w7z5/)。我将编写一个简单操作系统内核，让他在[LicheePi 4a](https://wiki.sipeed.com/hardware/zh/lichee/th1520/lp4a.html)上运行。ROM和uboot暂时用的是板卡emmc中自带的。

其中，01-08文件夹是在GROL-OS中逐步增加内容的过程，根目录下的其他文件是编译内和需要用到的文件们。doc文件夹中是本次工程需要用到的文档，包括C910cpu的用户手册、集成手册，以及[LicheePi4a的uboot的源代码。](https://github.com/revyos/thead-u-boot)

该项目的学习过程参考了[汪辰老师的慕课](https://www.bilibili.com/video/BV1Q5411w7z5/),以及该项目的[github链接](https://github.com/plctlab/riscv-operating-system-mooc)

我本硕学的是数字电路设计方向，完成过3次简单cpu的设计(mips,riscv,u-risc,都是课程大作业的那种)。同时组里也经常做一些linux下的C开发工作，熟练掌握CMake,Makefile这些东西。去年作了一些裸机代码的编写工作，对编译原理、交叉编译的过程有了大致的了解。我一直觉得我离打通软硬件只有一步之遥。于是我开始学习操作系统。学习操作系统还是很艰难的，尤其是前期想让板卡打印helloworld的时候。所以我就写个详细的教程，如果有人也和我一样想打通软硬件，不至于向我那会儿一样瞎摸索。

当然学习操作系统还有一个原因。因为我老婆生了，我感觉大家都在照顾孩子做家务的时候我要是打游戏那就太不是个东西了。没想到吧我研二就有女儿了。但我每天除了课题组的工作之外的确有很多时间空出来。就把GROL-OS的学习当作娱乐了。

以及为什么叫GROL-OS,因为我的女儿叫果儿。

还有linux的拼音输入法很废物，很难打。很多错别字。

### 进度概览
跟着[汪辰老师的慕课](https://www.bilibili.com/video/BV1Q5411w7z5/)走。

- 2024.6.26第一次更新，到了第十四章同步任务和锁。  
- 2024.7.2 第二次更新，完成了汪老师的课程内容十六章

### 前置知识

- RISC-V处理器的电路实现这部分内容我是在学校上课的时候学的，不知道什么网课比较好。不过这一部分不太重要。了解CPU内部构造即可，知道寄存器、cache、内存的区别。
- [RISC-V处理器的简单指令集](/doc/玄铁C910用户手册.pdf)见链接文档的第十四章
- Linux bash, Makefile, Git这些东西的知识很零碎。[汪辰老师的慕课](https://www.bilibili.com/video/BV1Q5411w7z5/)中会介绍这部分内容。[这个上交的视频](https://www.bilibili.com/video/BV1PL411M7bg)也可以。至于Git, 我感觉Github Desktop的傻瓜式操作非常友好。推荐。
- C/C++一些基本的操作就行。但要对指针的含义较为熟练。例如, `(type *)p + 1`和 `(type *)(p + sizeof(type))`是等价的；例如结构体变量中每一个组成元素的地址和结构体的地址的偏移关系。
- 计算机组成原理这部分我也是本科上的课。需要掌握计算机的内存空间的地址和数据的对应关系(一个地址对应一个byte)， 原码反码补码， 64位操作系统32位操作系统是什么意思等等。
- 编译原理
  这部分完全是我自己摸索的。我连专业的术语都不太清楚，只是对这个过程有个大概的了解。编译器会首先把C代码变成.s的汇编语言，然后变成.o的机器码。这个机器码是没有地址的，需要通过链接的操作连成一个完整的elf文件。[链接的规则就需要链接器脚本来实现](#连接器脚本的实现)，这一个过程之后会讲。

[上述内容中，重要的部分会在第一章中阐述。](#1前置知识补充)

### 这个工程要做的
- 参考[汪辰老师的慕课](https://www.bilibili.com/video/BV1Q5411w7z5/)，在qemu-32中跑通基本的操作系统
- 修改[github工程](https://github.com/plctlab/riscv-operating-system-mooc)，改称rv-64的操作系统内核
- 学习licheepi4a硬件手册，移植到上面
- 增加一些功能
- 学习markdown的撰写

### 这个工程会让人学到的


- 操作系统内核的相关知识用一些基础的数据结构来实现即可。
- 文档的阅读能力涉及到硬件实现，需要一直查文档。需要用到的文档都在/doc文件夹下。如果有需要，可以找到uboot的源代码，里面写了很多硬件寄存器的io映射。我会在这个readme里详细的写我是如何通过文档找到一些我想要的参数的。如果你用的不是LicheePi开发板，只要找到相关的详细文档（包括开发版的文档、CPU的文档，以及开发板自带的u-boot的源码），也能够boot起自己的操作系统。
- 各种硬件的驱动方式包括但不限于UART， PLIC， CLINT
- Markdown的基础语法
  显然我是为了学习markdown才写的这个readme。但这个readme好长。。。

## 1.前置知识补充

这些东西参考[b站mooc](https://www.bilibili.com/video/BV1Q5411w7z5/)的P1-P14

### C语言调用某一条汇编指令

在下文的directive环节中，我会讲述C和汇编如何互相调函数。这一部分单独讲如何在C中调用一句汇编。用的就是c中的asm关键字。例如

```c
static inline reg_t r_time(){
	reg_t ret;
	asm volatile("csrr %0, time" : "=r" (ret)); 
	return ret;
}
static inline void w_mtvec(reg_t x){
	asm volatile("csrw   mtvec, %0" :  :"r" (x):);
}
```

简单的寄存器操作只要抄上面这个模板就行。格式如下图所示![](/mdpic/1.png)

### 什么是栈

栈是一种先进后出的结构。一般有 `push`和 `pop`两种操作。例如一个空栈，我们顺序push 1，2，3，然后顺序pop，出来的顺序就是3， 2， 1.

栈这种结构是用来函数调用的。我第一次对栈有理解是在玩steam中的[图灵完备](https://store.steampowered.com/app/1444480/Turing_Complete/?l=schinese)游戏，游戏流程很短，需要电路基础，最后会让你在搭好的cpu上裸机执行汉诺塔。现在我们以简单的C语言递归算法实现斐波那契数列为例：

```c
int Fibonacci(int i){
    if (i <= 1) return 1;
    return Fibonacci(i-1) + Fibonacci(i-2);
}
```

如果我要计算 `Fibonacci(10)`,就得计算 `Fibonacci(9)`和 `Fibonacci(8)`。我们考察这个阶段：我们已经计算好了 `Fibonacci(9)`，即将调用 `Fibonacci(8)`。在进入 `Fibonacci(8)`的时候，我们得保存 `Fibonacci(9)`的结果，从而 `Fibonacci(8)`返回的时候，我们能将两者做个简单的加法并且返回。

我们可以发现，栈这种结构可以很好的实现这个过程。比如我们在 `Fibonacci(10)`中，想要进入 `Fibonacci(8)`,那就把所有 `Fibonacci(10)`的中间变量都压入栈中。在 `Fibonacci(8)`return时，就从栈中把中间变量全都取出，从而实现计算的过程。

关于栈的详细描述，我们可以参考[b站mooc](https://www.bilibili.com/video/BV1Q5411w7z5?p=9&vd_source=9bc606703e9bb90bffa41b4b15c117e4)。

### 汇编前置知识进阶

汇编语言基本上由汇编指令组成，例如：

```asm
addi    a0, a0, 4
//      a0 = a0 + 4
```

除此之外，汇编指令还有一些用.开头的指令，称为directive

#### directive指令

##### `.global`

```asm
.global myadd
myadd:
    addi    a0, a0, 4
    ret
```

`.global`指令表示后面跟着的函数名称可以被工程中所有别的文件调用，包括汇编文件和c文件。例如我有一个c文件定义了 `foo`函数：

```c
int foo(int a){
    return a + 1;
}
```

那我就可以通过 `.global`声明在汇编语言中调用这个函数,调用的过程使用call关键字。

```
.global foo
myadd:
	li	a0, 4
	call foo
	ret
```

##### `.align`

这个是用来对齐的。比如下面这条指令：

```asm
.align 4
    addi    a0, a0, 4
```

作用就是让 `addi`指令在内存中的地址4字节对其，也就是地址的最后两位二进制是0

##### 写数据相关

例如下面这段代码：

```asm
stack_start:
    .rept 12
    .word
    .endr
```

`.rept`的意思类似于for语句，意思就是重复 `.rept` `.endr`中间的语句12次。`.word`表示32位的0。因此这段话构造了一块12*32bit的空间。

##### `.equ`

类似于设置宏。比如，`.equ	STACK_SIZE, 1024`的意思就是之后出现 `STACK_SIZE`都表示1024.

#### RISC-V的通用寄存器

riscv64寄存器位宽64位，且每个寄存器都有约定俗成的作用，这个在汇编代码编写的时候尤其要注意，尤其是在函数调用的时候，要注意维护寄存器。

riscv64的cpu中有32个通用寄存器，且这些寄存器有两种表示方法。一种表示方法是 `x0~x31 `,表示的是寄存器的序号，在汇编语言转机器码的时候需要用到。注意 `x0`寄存器，无论怎样写，读出来都是0.另一种表示方法是按照寄存器不同的功能来区分的，这种方式称为ABI。例如，`sp`寄存器用来指向栈底，`fp`寄存器用来指向调用者函数的栈，`a0,a1...`是用来传递入参和返回值的，这种表示方法是用来写汇编语言用的。

这些通用寄存器两种方法的对应关系如下表所示：

|  名称  | ABI别名 |        描述        | 谁来维护 |
| :-----: | :-----: | :----------------: | :------: |
|   x0   |  zero  |      读恒为0      |   无需   |
|   x1   |   ra   |    函数返回地址    | 被调用者 |
|   x2   |   sp   |       栈指针       | 被调用者 |
|   x3   |   gp   |      全局指针      |  不重要  |
|   x4   |   tp   |     线程寄存器     |  不重要  |
|  x5~x7  |  t0~t2  |     临时寄存器     |  不用管  |
| x28~x31 |  t3~t6  |     临时寄存器     |  不用管  |
|   x8   |  s0/fp  | 指向上一层的栈地址 | 被调用者 |
|   x9   |   s1   |                    | 被调用者 |
| x10~x17 |  a0-a7  |    传递出入参数    |  调用者  |
| x18~x27 | s2~s11 |                    | 被调用者 |

对于传递函数参数：例如我有个函数 `int foo(int a, int b)`，那 `foo`函数被调用前，就需要把 `a0`寄存器赋值成 `a`,把 `a1`寄存器复制成 `b`，并在 `foo`函数的最后，将计算结果赋值回 `a0`。

对于 `sp`寄存器：参考[什么是栈](#什么是栈) 我们对栈的维护在被调用的函数中进行。以下面的汇编代码为例：

```asm
foo:
    //some modified to regester s5
    ...
    call foo1
    ...
    ret

foo1:
    addi    sp, sp, -8
    sd      s5, 0(sp)
    //do something
    ...
    ld      s5, 0(sp)
    addi    sp, sp, 8
    ret
```

在这段代码中，foo中会对 `s5`寄存器作一些修改，调用 `foo1`的时候，如果 `foo1`也对 `s5`寄存器有修改的话，返回的时候 `s5`就和之前不一样了，就会报错。因此，在执行 `foo1`的一开始，我们就需要把所有需要被调用函数维护的寄存器都保存在栈里。栈的地址是从上往下增长的。所以在 `foo1`的开头，我们把 `sp`寄存器减小了8. 8是因为一个地址代表一个byte，8个byte就是64字节，对应的是rv64寄存器也是64字节。然后把 `s5`寄存器存放到栈中，也就是 `sp`对应的位置。当 `foo1`执行完之后，我们就需要把栈里的东西都取出来。同时把 `sp`变回区。这个步骤就实现了中间量的保存。

对于 `ra`寄存器：这涉及到伪指令 `ret`和 `call`的工作原理。`ret`的含义是 `jalr x0, 0(ra)`，就是跳转到 `ra`寄存器指向的地址。`call foo`的含义是 `jalr ra, 0(foo)`, 也就是把 `ra`赋值成 `call`的下一条指令，然后跳转到 `foo`的位置。以下面这段汇编为例：

```asm
foo:
    ...
    aaa
    call foo1
    bbb
    ...
foo1:
    ...
    ccc
    call foo2:
    ddd
    ...
    ret
foo2:
    ...
```

在执行到 `ccc`的时候，`ra`指向的指令是 `bbb`,这是因为 `call foo1`的时候，`ra`被赋值了。所以我们希望 `foo1`在执行 `ret`的时候，`ra`是指向 `bbb`的，这样 `foo1`才能正常的返回 `foo`.但是当我们 `call foo2`的时候，我们改变了 `ra`的值，`ra`指向了 `ddd`。虽然这样能够让 `foo2`正常的返回 `ddd`,但 `foo1`就不能正常的返回 `bbb`了。所以我们在编写 `foo1`的时候，需要在一开始将 `ra`保存到栈中，并在 `ret`之前把 `ra`取回来。

#### RISCV 的控制状态寄存器（CSR）

这些寄存器具体内容查看[文档](/doc/玄铁C910用户手册.pdf)第16章。

这些寄存器可以读取或者改变CPU的状态，例如查看当前是cpu的第几个核，打开或者关闭相关的中断，等等等等。之后用到了在会讲。

第一个要用到的控制寄存器是 `mhartid`，m表示只有Machine模式的时候才可以访问这个寄存器，hart就是核心的意思。这个寄存器读他会返回是第几个hart。手册对这个寄存器的描述如下图所示：
![mhartid](/mdpic/2.jpg)

这些个寄存器不想通用寄存器，能通过addi等指令加减操作。他们更像内存中的数据，也要通过load store的指令放到通用寄存器中间接读写。不过CSR有专门的读写指令.

```asm
csrrw rd,csr,rs1
//csr <- rs1 && rd <- csr
----
csrrs rd,csr,rs1
//rd <- csr && csr <- csr|rs1
```

需要注意的是这两个指令都是原子的(Atomic)，也就是说不可分割，读写操作都是同时完成的，中断断不开
这两个操作为基础还有相关的伪操作：

```asm
csrw csr, rs
// equals to csrrw x0, csr, rs
//csr <- rs1
----
csrr rd, csr 
// equals to csrrs rd,csr, x0
//rd <- csr
```

综上所述，获取当前核心的ID号的C代码就可以这样实现：

```c
static inline reg_t r_mhartid(){
	reg_t x;
	asm volatile("csrr %0, mhartid" : "=r" (x) );
	return x;
}
```

#### 伪指令

上文讲道的 `call`和 `ret`就是伪指令。也就是说，伪指令是为了写汇编的时候方便，把多条指令用一条来表示的指令。但是gcc能够识别这个指令，并把他改写成正确的指令和机器码。除了 `call` `ret`,还有下述的常见伪指令：

##### `li`

在指令集中，没有加载大数的能力。例如要加载一个32位数 `0xdeadbeef`,需要两条指令

```asm
lui a0, 0xdeadb
addi a0, a0, 0xeef
```

所以就有了伪指令，一步到位：

```asm
li a0, 0xdeadbeef
```

他俩实际上是等价的

##### `la`

这个伪指令是用来加载地址的。等价于 `auipc`和 `addi`指令结合来生成一个地址。知道就行，不重要。

### 操作系统前置知识

#### 一个非常经典的问题：CPU上电后究竟干了什么

答：由于内存ram的数据每次上电都会丢，所以首先访问的肯定是rom

上电后cpu访问的地址是在电路设计的时候就设计好的，rom的位置也是电路设计的时候设计好的，参考vivado设计block design的时候需要填写address map中的地址。rom是一块很小的空间，只能执行有限的功能，其中最重要的是实现io的硬件映射。

所以此时，cpu就具有向串口输出debug信息的能力了。下面是LicheePi4A向串口输出rom调试信息。

```
brom_ver 8
[APP][E] protocol_connect failed, exit. 
```

除了串口驱动之外，rom还完成了一件事：执行外存的驱动。所以CPU还获得了读写外存(硬盘、EMMC、SDcard等)的能力。由于ROM的空间有限，既不能一次性完成所有IO的硬件映射，也不能一次性将整个内核读入内存。因此rom接下来作的事是将uboot读入内存。

uboot就厉害了，不但能完成IOMap，还能选择从哪儿读内核，还能联网，从网上读内核。具体怎么做之后在说。

然后uboot就把内核文件加载进内存的某个位置，然后跳转到这个地址就行了。

还是需要表扬一下RISCV架构。之前看过一门x86相关的网课，看了好久还在讲一些因为历史遗留问题而不得不进行的操作，很是劝退。RISCV就不用。

### 环境配置

- VMware17.0 + Ubuntu20.04这两者的安装不再赘述
- riscv64-unknown-elf- 工具链你可以下载原码来编译，也可以直接下载编译好的。推荐后者，[参考网站](https://nankai.gitbook.io/ucore-os-on-risc-v64/lab0/pei-zhi-huan-jing/an-zhuang-kai-fa-gong-ju)
- qemu-system-rv64 硬件模拟器
  可以用这个来验证mooc上的代码是否正确，然后再上板子。[参考网站](https://nankai.gitbook.io/ucore-os-on-risc-v64/lab0/pei-zhi-huan-jing/an-zhuang-ying-jian-mo-ni-qi)

## 2. Hello GROL-OS

这部分内容的代码请参考该项目的01文件夹，[汪辰老师慕课](https://www.bilibili.com/video/BV1Q5411w7z5/)中的P16-P17，[github](https://github.com/plctlab/riscv-operating-system-mooc)中的 `code/os/01-HelloRVOS`

### 框架

不管是qemu还是LicheePi,他们都有8个hart。上来就写多线程的操作系统不太好吧。所以我们就通过这段代码让其他核心空转，只保留0号核心来运行我们的操作系统。这部分代码参考01/start.S

```asm

_start:
	# park harts with id != 0
	csrr	t0, mhartid		# read current hart id
	mv	tp, t0			# keep CPU's hartid in its tp for later usage.
	bnez	t0, park		# if we're not on the hart 0
					# we park the hart
	# Setup stacks, the stack grows from bottom to top, so we put the
	# stack pointer to the very end of the stack range.
	slli	t0, t0, 10		# shift left the hart id by 1024
	la	sp, stacks + STACK_SIZE	# set the initial stack pointer
					# to the end of the first stack space
	add	sp, sp, t0		# move the current hart stack pointer
					# to its place in the stack space

	j	start_kernel		# hart 0 jump to c

park:
	wfi
	j	park

```

`wfi`指令是空转的意思。低功耗空转。同时这段代码还将栈均分成了8份。虽然我看没什么必要哈哈哈。

### UART操作-以qemu+ns16550a为例

cpu访问这些外设的操作都是大差不差的。一般都是这样：比如一个开关，他的0x0地址表示开，0x4地址表示关,这个0x0,0x4就表示偏置offset。这个偏置是多少是用来干什么的和操作系统无关，和cpu无关，只和这个硬件有关。然后操作系统在boot的过程中，或是rom，或是uboot，会给这个硬件分配一个基地址base address。那么cpu在访问这个硬件的时候，就可以用 `base_address + offset`的方式来读写硬件的寄存器，从而实现对硬件的操作。

首先我们需要想办法找到系统给的base_address.参考qemu源代码中的描述,找到UART0的基地址 `0x10000000`
![qemu-io](/mdpic/3.png)

```c
#define UART0 0x10000000
```

然后来学习uart的寄存器操作。ns16550a可以参考[这个文档](/doc/mooc/td16550.pdf)。首先可以将各个寄存器的offset写进define中

```c
#define UART_REG(reg) ((volatile uint8_t *)(UART0 + reg ))
#define RHR 0	// Receive Holding Register (read mode)
#define THR 0	// Transmit Holding Register (write mode)
#define DLL 0	// LSB of Divisor Latch (write mode)
#define IER 1	// Interrupt Enable Register (write mode)
#define DLM 1	// MSB of Divisor Latch (write mode)
#define FCR 2	// FIFO Control Register (write mode)
#define ISR 2	// Interrupt Status Register (read mode)
#define LCR 3	// Line Control Register
#define MCR 4	// Modem Control Register
#define LSR 5	// Line Status Register
#define MSR 6	// Modem Status Register
#define SPR 7	// ScratchPad Register
```

然后我们就可以通过读写 `UART_REG(XXX)`来读写UART的相关寄存器了

#### 初始化UART

可以对照着[文档](/doc/mooc/td16550.pdf)来看为什么要这样初始化。

```c
/* 关闭中断，我们暂时还用不到 */
	uart_write_reg(IER, 0x00);

/* 设置波特率 */
	uint8_t lcr = uart_read_reg(LCR);
	uart_write_reg(LCR, lcr | (1 << 7));//设置波特率开关，表示接下来要设置波特率了
	uart_write_reg(DLL, 0x03);
	uart_write_reg(DLM, 0x00);
    //参考文档，DLL和DLM这样设置能把波特率设置成115200


/* 设置停止位、奇偶校验位 */
	lcr = 0;
	uart_write_reg(LCR, lcr | (3 << 0));
```

#### UART输出字符

可以对照着[文档](/doc/mooc/td16550.pdf)来看为什么要这样输出字符。

```c
#define LSR_RX_READY (1 << 0)
#define LSR_TX_IDLE  (1 << 5)
int uart_putc(char ch)
{
	while ((uart_read_reg(LSR) & LSR_TX_IDLE) == 0);
	return uart_write_reg(THR, ch);
}

void uart_puts(char *s)
{
	while (*s) {
		uart_putc(*s++);
	}
}//打印字符串

char uart_getc(){
	while ((uart_read_reg(LSR) & LSR_RX_READY) == 0);
	return uart_read_reg(RHR);
}//等待一个字符的输入

uart_puts("hello world!");
```

### Makefile 究竟是干什么的

makefile的核心语法就是：

```makefile
{target}:{prequisites}
{command}
#command 就是一些bash中的语法指令。这里不多说了
```

并且辅之以一堆字符串处理的函数

对于01文件夹中的工程，主makefile在 `01/Makefile`，这个文件中有三个等式

```makefile
USE_LINKER_SCRIPT = false

SRCS_ASM = \
	start.S \

SRCS_C = \
	kernel.c \
	uart.c \

include ../common.mk
```

其中，`SRC_S` `SRC_ASM`都是字符串数组，里面的每个字符串都代表了一个文件的路径。这两个变量通过 `${SRC_S}`这样的方式来引用。然后include了根目录下的 `common0.mk`

`common0.mk`中首先有很多字符串的赋值操作。挑选几个有代表性的来讲：

```makefile
CROSS_COMPILE = riscv64-unknown-elf-
#这句话代表字符串数组赋值， 创建了一个长度为一的字符串数组
CFLAGS += -nostdlib -fno-builtin -g -Wall
#这句话的意思是扩展字符串数组，扩展了4个字符串
CC = ${CROSS_COMPILE}gcc
#这句话的意思是字符串拼接，在${CROSS_COMPILE}这个字符串数组的最后一个字符串的最后添加gcc三个字符。
```

以及复杂的字符串操作。先举一个例子

```makefile
OBJS_ASM := $(addprefix ${OUTPUT_PATH}/, $(patsubst %.S, %.o, ${SRCS_ASM}))
#patsubst是改后缀。%是占位符，类似shell中的*
#$(patsubst %.S, %.o, ${SRCS_ASM})的意思是：返回一个字符串数组，这个字符串数组是${SRCS_ASM}中所有的什么什么.S全都替换成什么什么.o。这是对字符串数组中的每个字符串都做这样的操作。
#addprefix 是增加前缀的意思。$(addprefix ${a}, ${b})的意思就是${b}中每个字符串都加上${a}的前缀
#字符串数组可以视作空格链接的一个字符串。
```

然后是和target相关的操作

```makefile
.default_target := xxx
#顾名思义就是把xxx当成默认的target，也就是说在shell中输入make所执行的target

all: ${OUTPUT_PATH} ${BIN_PATH} ${ELF}
#说明all这个target要生成后面这些个target
#值得一提的是，如果后面的target已经存在，那么make就不会去执行存在的target
#但如果有些target不生成目标呢？

.PHONY yyy
#这句话就说明yyy是步生成目标的target。无论他的目标有没有，make yyy都会被执行。

```

### qemu仿真

到这里就可以进行qemu仿真了。qemu仿真由两个步骤组成，参考Common0.mk中的相关内容

```makefile
	@${QEMU} ${QFLAGS} -kernel ${ELF} -s -S &
	@${GDB} ${ELF} -q -x ../gdbinit
```

第一句话的意思是：启动qemu仿真器，-s表示将调试的信息发送到1234端口，-S表示启动仿真器之后马上暂停。
第二句话的意思是启动gdb调试器。qemu的仿真必须通过gdb的方式来进行调试。接下来看看 `gdbinit`中的内容

```makefile
set disassemble-next-line on
b _start
target remote : 1234
c
```

第一句话说的是gdb中每一个机器码就显示源代码，不重要
第二句话是说明在_start处打一个端点
第三句话是链接之前qemu的1234端口
第四句话是运行到断点处
之后可以在gdb的窗口输入si，就是步进的意思，一次执行一条机器码;也可以输入c，直接运行全部。

注意：qemu仿真不要用这个文档中的代码，要用[汪辰老师的慕课](https://github.com/plctlab/riscv-operating-system-mooc)中的代码。

不过mooc中的代码是rv32的，如果要在rv64的qemu中运行，需要改common.mk中的10-16行：

```makefile
CFLAGS += -nostdlib -fno-builtin -g -Wall
CFLAGS += -march=rv64gc -mabi=lp64 -mcmodel=medany
#-march 改称rv64gc，这个表示指令集。gc也是c910的指令集
QEMU = qemu-system-riscv64
#qemu改称rv64的qemu
QFLAGS = -nographic -smp 1 -machine virt -bios none

GDB = ${CROSS_COMPILE}gdb
#gdb改为交叉编译中的gdb
```

然后在01-HelloRVOS/下运行 `make debug` 就能输出正确结果了。

```shell
uos@ubuntu:~/riscv-operating-system-mooc/code/os/01-helloRVOS$ make debug
Press Ctrl-C and then input 'quit' to exit GDB and QEMU
-------------------------------------------------------
Reading symbols from out/os.elf...
Breakpoint 1 at 0x80000000: file start.S, line 11.
0x0000000000001000 in ?? ()
=> 0x0000000000001000:	97 02 00 00	auipc	t0,0x0

Breakpoint 1, _start () at start.S:11
11		csrr	t0, mhartid		# read current hart id
=> 0x0000000080000000 <_start+0>:	f3 22 40 f1	csrr	t0,mhartid
(gdb) c
Continuing.
Hello, RVOS!
```

### 上板运行

#### 修改UART的寄存器offset

显然，不同的串口芯片的寄存器是不一样的。我们需要从LicheePi的[外围接口文档](/doc/玄铁曳影1520芯片原型外围接口用户手册_v1.0_cn.pdf)中找到该uart的寄存器offset分配。
经过对比后，我发现这个uart和qemu的串口寄存器没有太大的区别。唯一的区别是：qemu中的串口寄存器都是8位的，但是LicheePi上的寄存器都是32位的，所以两个寄存器之间的差是4不是1.因此需要修改 `uart.c`中的这一行代码：

```c
//#define UART_REG(reg) ((volatile uint8_t *)(UART0 + reg))

#define UART_REG(reg) ((volatile uint8_t *)(UART0 + reg * 4))
```

#### 修改UART的基地址

不同的rom给UART分配的串口基地址也是不一样的。以下给出找到外设基地址的3种办法：

- 查阅LicheePi的[系统用户手册](/doc/玄铁曳影1520芯片原型系统用户手册_v1.0_cn.pdf)，在1.1中有C910的地址映射表，表中显示：

|    起始地址    |    结束地址    | 寻址空间 | 描述 | CACHE |
| :------------: | :------------: | :------: | :---: | :---: |
| 0xFF_E701_4000 | 0xFF_E701_7FFF |   16KB   | UART0 |      |

- 查阅[thead-u-boot源码](https://github.com/revyos/thead-u-boot)，其中 `/arch/dts/riscv/light-lpi4a.dts`是LicheePi4A的uboot设备树文件，描述了各个硬件在内存中的基地址。可以找到里面对UART0的描述
  ```
    serial@ffe7014000 {
        compatible = "snps,dw-apb-uart";
        reg = <0xff 0xe7014000 0x0 0x400>;
        clocks = <&dummy_uart_sclk>;
        clock-frequency = <100000000>;
        clock-names = "baudclk";
        reg-shift = <2>;
        reg-io-width = <4>;
        u-boot,dm-pre-reloc;
    };
  ```
- 也可以查阅[thead-u-boot源码](https://github.com/revyos/thead-u-boot)其中的 `drivers/trng/soc.h`,里面也定义了基地址
  ```c
    #define DW_UART0_BASE               0xFFE7014000UL
  ```

宗上，在LicheePi中，UART0的基地址是 `0xFF_E701_4000`,我们需要修改的代码是01/platform.h

```c
#define UART0 0xffe7014000L
//#define UART0 0x10000000L
```

#### 链接串口线

首先，阅读LicheePi4A官方文档的[外设使用](https://wiki.sipeed.com/hardware/zh/lichee/th1520/lpi4a/6_peripheral.html)，里面的第四部分讲述了UART的接口。这块开发板的系统串口没有改成usb管脚，通过gpio的方式引了出来。因此我们需要像文档里写的一样，用杜邦线把 `U0-RX` `U0-TX` `GND`三个管脚和官方送的 `RV-Debugger`链接。

- 需要注意：RX连TX，TX连RX
- 如果没有 `RV-Debugger`，可以自己买一个 `FT232RL`下载器，淘宝上只要五六块，这个东西也是串口转usb的。
- 不用连杜邦线，`RV-Debugger`或者 `FT232RL`就会亮LED灯。如果没有亮，可能有以下两种情况：
  - usb线不对，比如typec的线有的只能充电，有的能传数据。
  - `RV-Debugger`模块坏了
    不幸的是这两种情况我都遇到过。大家要保管好 `RV-Debugger`,不要像我一样被猫玩弄并且被扫地机器人吸入。

#### 启动uboot

然后选择LicheePi的启动模式。卸下SOC板，可以看到非常明显的位置有两个开关，开关边上也印上了这两个开关的含义。
![开关](/mdpic/5.jpg)
所以究竟是选择SD卡启动还是emmc启动呢？
回到之前说的，CPU上电后干什么的问题。CPU上电后跑rom，rom跑完后，通过这个开关，判断到底是去SD卡还是去emmc跑uboot。LicheePi在emmc中有自带的uboot。我们之前的代码工作只是写了内核，并没有写u-boot。所以我们要从emmc启动，借助emmc中的uboot来启动我们的内核。

上电，链接串口线，打开xshell（如何使用xshell我就不说了），系统串口依次输出下列内容

- brom信息
  ```
    brom_ver 8
    [APP][E] protocol_connect failed, exit. 
  ```
- uboot SPL(Second Program Loader)
  ```
    U-Boot SPL 2020.01-gd6c9182f (Jul 07 2023 - 12:31:51 +0200)
    FM[1] lpddr4x dualrank freq=3733 64bit dbi_off=n sdram init
    ddr initialized, jump to uboot
    image has no header
  ```
- uboot
  ```
    U-Boot 2020.01-gd6c9182f (Jul 07 2023 - 12:31:51 +0200)

    CPU:   rv64imafdcvsu
    Model: T-HEAD c910 light
    DRAM:  8 GiB
    C910 CPU FREQ: 750MHz
    AHB2_CPUSYS_HCLK FREQ: 250MHz
    AHB3_CPUSYS_PCLK FREQ: 125MHz
    PERISYS_AHB_HCLK FREQ: 250MHz
    PERISYS_APB_PCLK FREQ: 62MHz
    GMAC PLL POSTDIV FREQ: 1000MHZ
    DPU0 PLL POSTDIV FREQ: 1188MHZ
    DPU1 PLL POSTDIV FREQ: 1188MHZ
    MMC:   sdhci@ffe7080000: 0, sd@ffe7090000: 1
    Loading Environment from MMC... OK
    Error reading output register
    Warning: cannot get lcd-en GPIO
    LCD panel cannot be found : -121
    splash screen startup cost 15 ms
    In:    serial
    Out:   serial
    Err:   serial
    Net:   ethernet@ffe7070000 (eth0) using MAC address - ae:be:2a:50:5e:89
    eth0: ethernet@ffe7070000ethernet@ffe7070000:0 is connected to ethernet@ffe7070000.
    ethernet@ffe7060000 (eth1) using MAC address - ae:be:2a:50:5e:8a
    , eth1: ethernet@ffe7060000
    Hit any key to stop autoboot:  2
  ```

这会儿就要注意了！你只有两秒钟的时间进入uboot。不然就按照默认流程，uboot自动启动默认的linux内核。

#### 如何在uboot中启动内核

这时候我们应该在uboot的命令行中了。可以输入help，结合百度，探索uboot里的指令都可以干什么，有什么用。by the way, 在这里输入 `coninfo`也可以显示串口的物理地址。

```shell
C910 Light# coninfo
List of available devices:
serial@ffe7014000 00000007 IO 
serial   00000003 IO stdin stdout stderr 
nulldev  00000003 IO 
```

uboot其实是拥有读写sd卡的能力的。所以我的planA是将内核的elf文件放在sd卡里，然后直接运行。但吊诡的是我无论怎么研究，都无法正常读取sd卡。于是我就放弃了这个方案。

在输入help指令后，我发现uboot有 `tftp`指令，也就是说他可以通过网络，把其他地方的elf文件读到内存的某个位置（默认0xc0100000），然后通过 `bootelf`指令执行某个位置的elf文件（也是默认0xc0100000）。为了实现这个方案，我需要在电脑上构建一个tftp服务器，把 `make`生成的elf文件放到tftp服务器的目录下。这样uboot的 `tftp`命令就可以获取到电脑上的elf文件了。但是不知道为什么，我在linux上搭建的服务器uboot无法访问。于是我最终搭载了windows上。具体流程如下

- windows下构建tftp服务器需要下载一个软件[tftpd64](https://pjo2.github.io/tftpd64/),并按照下图配置。`Current Directory`随便选，就是之后把elf文件拷贝近来的文件夹；`Server interfaces`选择的时候需要注意：要选择能和板卡在同一个网段下的网卡。然后这个窗口挂着就行，这就代表windows开启了这个服务。    
![tftpd64](/mdpic/4.jpg)
- 将虚拟机中 `make`生成的 `01/out/GROL.elf`文件拷贝到上一部的 `Current Directory`中。
- 进入uboot，输入 `tftp GROL.elf`,内核可执行文件就被暂存到 `0xc0100000`中了。这时候uboot窗口会打印如下。
  ```
    C910 Light# tftp GROL.elf
    Speed: 1000, full duplex
    Using ethernet@ffe7070000 device
    TFTP from server 192.168.31.150; our IP address is 192.168.31.46
    Filename 'GROL.elf'.
    Load address: 0xc0100000
    Loading: #####
        5 MiB/s
    done
    Bytes transferred = 72688 (11bf0 hex)
  ```
- 输入 `bootelf`，也就是执行 `0xc0100000`中的可执行文件。至此，我们完成了helloworld。每按一次回车，都会打印出一行 `hello, GROL-OS!`
  ```
    C910 Light# bootelf
    ## Starting application at 0x80000000 ...
    Hello, GROL-OS!
    Hello, GROL-OS!
    Hello, GROL-OS!
    Hello, GROL-OS!
  ```

#### 最后的小问题

也许有些同学会遇到这样的问题，那就是最后输出的时候没有对齐：

```
    C910 Light# bootelf
    ## Starting application at 0x80000000 ...
    Hello, GROL-OS!
                   Hello, GROL-OS!
                                  Hello, GROL-OS!
                                                 Hello, GROL-OS!
```

这是因为：你一定在 `kernel.c`中写了

```c
uart_puts("Hello, GROL-OS!\n");
//而不是
uart_puts("Hello, GROL-OS!\n\r");
```

这是因为：`\n`只是把光表移到正下方，`\r`是把光标移到这一行的最前面。所以当我们要按回车的时候，一定要用 `\n\r`来表示。关于如何将字符显示在想要的位置，我会在之后讲。

## 3. printf && page alloc

这一部分内容对应的是[汪辰老师慕课](https://www.bilibili.com/video/BV1Q5411w7z5/)中的P18，第8章内存管理，[github](https://github.com/plctlab/riscv-operating-system-mooc)中的 `code/os/02-memanagement`,以及本项目中的/02/

### C语言中变量与内存的关系

C语言中不同种类的变量会放在不同的地方。

- 自动变量（局部变量），也就是有作用域的变量，被大括号包住的变量。由[栈](#什么是栈)管理。进入作用域的时候在寄存器中，调用别的函数的时候就会被存放到栈里。退出作用域的时候就从栈中pop出来，被释放。
- 全局变量，静态变量，放在**数据段**中。数据段会在下一部分中介绍。
- 动态变量，也就是 `malloc` `free` 的变量，由堆管理，要用的时候从这里申请，用完了就释放。堆和栈是连在一起的，堆从低往高增长，栈从高往低增长，两边碰到一起了就说明空间用完了，就报错了。

这一章的内容就是要实现动态内存的分配。但mooc的这一章我觉得作的有点不好，因为最终没有实现malloc接口，我也不知道该怎么实现，所以感觉有点莫名其妙。最后也没花心思去研究怎么实现按字节分配内存，简单实现了就烂在这里了。给汪老师提一个建议，这部分可以多讲一点，最好能实现malloc。

### 连接器脚本的实现

我们继续查看 `common0.mk`文档，看看里面的这几句话：

```makefile
USE_LINKER_SCRIPT ?= true
ifeq (${USE_LINKER_SCRIPT}, true)
LDFLAGS = -T ${OUTPUT_PATH}/os.ld.generated
else
LDFLAGS = -Ttext=0x80000000
endif
```

`?=`操作符的意思是：如果这个变量没有被定义过，就用后面的值赋值给他。在01文件夹的makefile中有对 `USE_LINKER_SCRIPT`变量的定义，因此会执行 `else`中的这句话，就是把elf中的机器码从 `0x80000000`开始往后放，不太重要。从这一章开始,`USE_LINKER_SCRIPT`变量不被定义，因此执行 `ifeq`后的这条指令，也就是把 `os.ld.generated`作为连接器脚本。而这个链接器脚本是根据下面这句话产生的。

```makefile
ifeq (${USE_LINKER_SCRIPT}, true)
	${CC} -E -P -x c ${DEFS} ${CFLAGS} scripts/os.ld > ${OUTPUT_PATH}/os.ld.generated
endif
```

也就是根据 `scripts/os.ld`产生的。其实 `os.ld` `os.ld.generated` 没什么本质区别，因此之后我们就对着 `os.ld`看就行。

#### 链接器脚本的作用

首先简单讲一下链接器脚本是用来干什么的。可执行文件elf中不一定只有代码，还有一些只读变量（比如上一章中的"Hello, GROL-OS!"字符串），一些静态变量，等等。连接器脚本的目的就是让可执行文件变得整齐，比如代码都放在一起，放在什么地方；静态变量都放在一起，放在什么地方，等等等等。都放在一起的这一堆东西，就叫段。比如代码放在一起，就叫代码段 `.text`。常见的段的作用和名称如下所示：

|     代码段     |  `.text`  |         放代码的地方         |
| :------------: | :---------: | :--------------------------: |
|     数据段     |  `.data`  | 放被初始化过的全局变量的地方 |
|   只读数据段   | `.rodata` |  只读，比如const类型的变量  |
| 未初始化数据段 |  `.bss`  | 放未被初始化的全局变量的地方 |

我们可以进入LicheePi的Linux内核，通过 `cat /proc/iomem`指令，查看内核在内存中的分布。（这里也可以看出串口等外设的 `base_address`）
  
  ![iomem](/mdpic/6.jpg)

可以看到，内核被加载到了 `0x002_00000`，一直到 `0xffff_ffff`一共大概4.2G的空间都是给内核用的。bss之后很大的一段空间就是给堆和栈的。

#### 链接器脚本代码解析

解析 `02/scripts/os.ld`的代码。

```
#include "platform.h"
OUTPUT_ARCH( "riscv" )
ENTRY( _start )
```

前两句话顾名思义，第三句话挺重要。他定义了入口函数（其实 `_start`作为入口函数是默认的，不过不重要）。入口函数的意思是，这个可执行文件一执行，首先去找 `_start`。（好像很快就能说明白，也没那么重要）

```
MEMORY
{
	ram   (wxa!ri) : ORIGIN = 0x80000000, LENGTH = LENGTH_RAM
}
```

这一部分也很重要，他是用来定义这段可执行文件执行的时候可以被装载的位置。这句话的意思就是，定义了一块叫做 `ram`的空间，这块空间从 `0x80000000`开始且长度为 `LENGTH`，`(wxa!ri)`中描述的是这块空间的权限，可读可写可执行这类的。

最后一块内容叫做 `SECTION`,是最重要的部分。里面经常出现 `PROVIDE`关键字。

```
PROVIDE(_data_end = .);
```

这句话也相当于是定义全局变量。这句话定义了一个叫做 `_data_end`的全局变量。在这块内容中，所有出现的单独的 `.`都表示当前的地址。比如下面这段代码

```
.data : {
    /*
        * . = ALIGN(4096) tells the linker to align the current memory
        * location to 4096 bytes. This will insert padding bytes until
        * current location becomes aligned on 4096-byte boundary.
        * This is because our paging system's resolution is 4,096 bytes.
        */
    . = ALIGN(4096);
    PROVIDE(_data_start = .);
    /*
        * sdata and data are essentially the same thing. We do not need
        * to distinguish sdata from data.
        */
    *(.sdata .sdata.*)
    *(.data .data.*)
    PROVIDE(_data_end = .);
} >ram
```

这段代码的意思是在最后生成的可执行文件中生成一个data段，data段里的东西都是从之前的.o文件中找data段，然后拼起来的。
这段代码从上往下执行，所以 `.`一开始的值就是 `data`段开头的值。然后执行 `. = ALIGN(4096)`,align就是对齐的意思。所以 `.`就往后挪到了第一个4096对其的位置，也就是 `0x*****000`的位置。然后 `PROVIDE(_data_start = .)`就把这个位置保存给了变量 `_data_start`。
之后的这句话 `*(.sdata .sdata.*)`可以看成乘法分配率，也就是 `*.sdata *.sdata.*`，其中的 `*`号就是Linux的Shell中的 `*`,也就是占位符，表示任意的字符串。所以这句话的意思是，从 `_data_start`这个位置开始，把所有 `.o`文件中所有后缀为 `.sdata`,或者名字中间有 `.sdata.`的段都放在这里。放完之后，`.`就移动到这些段的末尾。然后再放后缀为 `.data`或者中间有 `.data.`的段，然后再把 `.`移动到这些段的末尾。到这里为止，最终的可执行文件的 `data`段就算是分配完成了，我们再把这个位置赋值给变量 `_data_end`。
大括号最后有个 `>ram`,这个 `ram`就是之前提到的 `MEMORY`这段代码中定义的一块内存。段就通过这样的方式被放到内存中。
当然，除了 `. = ALIGN(4096)`，还有更直接的方式改变当前的地址。比如 `. = 0x00200000`就能直接把当前地址切换到特定的位置。

#### 链接器脚本中的变量调用

参考 `02/mem.S`，这个文档描述了一个 `.rodata`段，里面定义了很多全局常量。这些常量的初值就是链接器脚本中 `PROVIDE`关键字定义的。

### printf的实现

之前的 `uart_puts`已经能够很好的打印字符串了。但我们回忆以下在写C语言的时候，我们可以用 `printf`来打印数字。例如：

```c
int __version__ = 2;
printf("Hello GROL-OS. version %d\n\r", __version__);
```

汪老师提供的代码中将 `printf`函数封装在了 `02/printf.c`文件中，具体实现方式我就不详细写了。

### 在堆中实现页(4K)的分配

这部分内容主要讲述了一个简单的页分配算法，[汪辰老师的慕课](https://www.bilibili.com/video/BV1Q5411w7z5/)讲述的很详细，结合代码就能看明白，我就不说了。（主要是发现我当时没记笔记）不过我认为这些是不够的。我将在之后再学习别的老师的这方面的内容，然后对GROL-OS进行改进。我预设的目标是：封装出一个和C中 `malloc` `free`功能一样的函数。

### 修改Makefile

虽然这部分我没有深入的学习下去，不过我还是意识到了一个可以改进的点：这一章的 `page_init` `page_test`等函数打印了很多测试信息，写了很多 `printf`语句。我最后生成的 `GROL-OS`完整版肯定不需要这些打印。但是这些 `printf`语句一条条删除肯定很麻烦。因此我想到了一个办法：把所有的 `printf`都放到 `#ifndef`语句中，并在 `common.mk`中选择性的定义宏。
比如，如果我运行 `make`，那我就不定义 `MYPRINT`这个宏，内核中的所有调试信息 `printf`都无法执行；但如果我运行 `make myprint`，makefile就会帮我定义 `MYPRINT`这个宏，那么内核的调试信息就都会打印出来。这样就会省下很多的工作量。
修改后的代码参考 `common.mk`，其中修改的部分主要集中在46/47两行

```makefile
myprint:CFLAGS+=-DMYPRINT
myprint:${OUTPUT_PATH} ${BIN_PATH} ${ELF}
```

这涉及到一个gcc的语法：`-DXXXX`就是定义一个 `XXXX`的宏。

## 4. 协作式多任务

这一部分内容对应的是[汪辰老师慕课](https://www.bilibili.com/video/BV1Q5411w7z5/)中的P19，第9章协作式多任务，[github](https://github.com/plctlab/riscv-operating-system-mooc)中的 `code/os/03-contextswitch` `04-multitask`,以及本项目中的/04/。

### 多任务

我们在用操作系统的时候肯定有很多任务在同时进行。比如浏览器正在播放视频的时候右下角的时钟在走。理论上一个CPU的hart只能作一件事，CPU中的hart数目是有限的，但是我想让他同时做的任务是没有之境的。比如我的i5-13600kf有6P+8E一共20个hart，但我可以同时看21个视频也没有问题。也就是说，有一个技术能够让一个hart同时作多个任务。一个hart做多个任务，这就叫做**多任务**。

多任务的实现乍一想其实很简单：task1执行一会儿后，cpu把寄存器里的数据都存到内存的某个角落，然后从内存的另一个角落里找到task2之前存的寄存器，并开始执行，循环往复。这个也是现在多任务实现的核心思想。接下来就是一些细节上的问题了。

比较重要的一个问题就是：谁来判断task1该不该让出执行权。由此引出了两种不同的多任务实现方式：

- 抢占式多任务：由内核来控制每个任务该做多久，然后强制让这个任务下线，另一个任务上线。
- 协作式多任务：每个人物自己来执行切换的任务。task1感觉自己差不多该换了，就让出自己的控制权，交给下一个人来执行任务。

虽然协作式多任务看起来比较民主，但是有一个问题：他非常依赖程序编写者的能力与大局观。毕竟多做多错，什么时候让出控制权，怎么让出控制权，怎样避免控制权切换的时候出现的bug，总之增加了程序员的工作量。同时，还会有恶意的程序，完全不管其他任务的死活，就占着执行权，直到自己就结束了才让出。或者强制占用cpu不退出，导致电脑死机啥的，总之有很多问题。
相比之下，抢占式多任务就好的多。有个东西会帮CPU记着时间，时间一到就通知CPU。CPU就赶紧切换任务。这个过程对于任务来讲是不存在的，任务傻乎乎的以为自己独占了CPU资源，其实CPU已经换着执行了一大批任务了。这种方式减轻了程序员编写任务的压力，避免了一些安全上的问题。怎么看都很完美。

但是抢占式多任务需要用到一个叫做**中断**的全新机制，也需要除了CPU外其他硬件的支持。我们就从简入难，先实现协作式多任务。等到之后，我们会修改我们的多任务实现算法的。

### 上下文 && mscratch 寄存器

我在前面提到，多任务实现的核心思想就是：task1执行一会儿后，cpu把寄存器里的数据都存到**内存的某个角落**，然后从**内存的另一个角落**里找到task2之前存的寄存器，并开始执行，循环往复。
我们把寄存器的数据都存到内存的某个角落，这些数据在内存中就叫做**上下文**(context)。上下文用一个结构体整齐的保存就行：

```c
struct context {
	/* ignore x0 */
	reg_t ra;
	reg_t sp;
	reg_t gp;
	reg_t tp;
      ......
	reg_t t3;
	reg_t t4;
	reg_t t5;
	reg_t t6;
};

struct context ctx_tasks[MAX_TASKS];
//由于我们还没有malloc，之后所有东西的实现方式就都不用链表了。全部用数组来表示。
```

仅仅有上下文还是不够的。比如taskn他感觉他要让出控制权了，那他得把自己的寄存器存到上下文中去。他怎么知道自己的上下文地址呢？riscv架构提供了这样的一个特权寄存器 `mscratch`，他就是专门用来干这个的，他的数据完全依赖内核，硬件不会动他。内核要做的事情也很简单：在要切换上下文之前，根据自己的 `mscratch`，找到下一个 `mscratch`，这之后再切换。


切换的过程是怎么实现的呢？参考之前在[通用寄存器](#risc-v的通用寄存器)章节提到过的 `ra`寄存器与 `ret`的实现。在汇编语言中，一个函数最终通常由伪指令 `ret`返回调用它的函数，而这个返回地址就存在 `ra`寄存器当中。所以，在切换上下文的时候，我们为了跳转，只要更改 `ra`的值，然后通过 `ret`指令，就能实现上下文的切换了。所以为了上下文切换，我们需要做的事情流程如下：

1. 在上下文数组中找到下一个要执行的任务，传递给上下文切换函数。
2. 根据 `mscratch`通用寄存器的值找到上下文数组，把当前状态下CPU的所有寄存器都存储到上下文结构体中。
3. 用传入的上下文地址更新 `mscratch`。
4. 用新任务的上下文代替CPU中所有通用寄存器的值（这时候就已经更新 `ra`了）。
5. 执行 `ret`指令，实现上下文的切换。

这部分的代码放在 `/04/entry.S`中，是用汇编实现的函数 `switch_to`

```asm
switch_to:
	csrrw	t6, mscratch, t6	# swap t6 and mscratch
	beqz	t6, 1f			# Note: the first time switch_to() is
	                                # called, mscratch is initialized as zero
					# (in sched_init()), which makes t6 zero,
					# and that's the special case we have to
					# handle with t6
	reg_save t6			# save context of prev task

	# Save the actual t6 register, which we swapped into
	# mscratch
	mv	t5, t6			# t5 points to the context of current task
	csrr	t6, mscratch		# read t6 back from mscratch
	STORE	t6, 30*SIZE_REG(t5)	# save t6 with t5 as base

1:
	# switch mscratch to point to the context of the next task
	csrw	mscratch, a0

	# Restore all GP registers
	# Use t6 to point to the context of the new task
	mv	t6, a0
	reg_restore t6

	# Do actual context switching.
	ret

.end
```
`reg_save` `reg_restore`也是用汇编写的函数。他们的作用是储存/加载CPU的通用寄存器到某个地址。我就不妨在这里了，感兴趣的可以去`/04/entry.S`中去看。  
需要注意的是这段代码的前几行，`csrrw`指令的作用是交换`t6` `mscratch`,因此执行完`reg_save`指令后，`context`中的`t6`并不是我们想要的`t6`,我们想要的`t6`被放在了`mscratch`中。所以需要通过之后的几条代码找回`t6`寄存器。 

再强调以下，`a0`寄存器是用来传递参数的。他就是`extern void switch_to(struct context *next)`中传递进来的结构体指针，指向要切换到的上下文。

`switch_to`函数只实现了上面列表中的2-5的功能。如何实现1的功能呢？在汪老师的代码中，汪老师是直接让用户任务“开天眼”，比如task1自己知道要传给task2，task2自己也知道要传给task1.其实我们可以采取一个更加好的方案，也就是汪老师在课后习题中要求完成的方案：构建一个特殊任务`task_controller`,task1结束后直接返回`task_controller`，由`task_controller`分配谁才应该是下一个该进行的任务。`task_controller`的实现代码在`/04/sched.c`中：
```c
static void task_controller(void* param){
	static int next_task = 0;
	if (_num <= 1) {
		panic("Num of task should be greater than zero!");
		return;
	}
	while (1){
		printf("I am task controller!\n\r");
		task_delay(DELAY);
		next_task  = next_task % MAX_TASKS + 1;
		while (ctx_tasks[next_task].flags == 0) next_task  = next_task % MAX_TASKS + 1;
		printf("I am goint to switch to %dth task\n\r", next_task);
		struct context * tmpcontext = &ctx_tasks[next_task];
		switch_to(tmpcontext);
	}
}
```
这有个`next_task`变量，也就是说，`task_controller`储存了当前在运行的是哪个任务。这个人物交出控制权后，他往后一个个找合适的任务并执行就行。由于我们用的是数组来存储上下文，所以我们需要一个`flag`来表示这个任务到底有没有存在。参考课后练习，再放一个`priority`变量，但我在这一讲中不实现优先级的功能，之后抢占式多任务的时候再实现。

有了上述的flag，就能实现任务的推出了。只要记得在退出的时候维护`context`结构体就行。
```c
void task_exit(){
	struct context* pmycontext = r_mscratch();
	pmycontext->flags = 0;
	_num--;
	schedule();
}
```

### 一些娱乐
既然是操作系统，应该有花里胡哨的字符画啥的。但我技术有限，画的还是十分朴素。代码放在`/04/graphprint/`中,之后有空再想着去改进他把。我就不贴出来了。

## 5. 中断和异常

这一章的内容非常多，看来我的内容分布出了问题。涉及到软件设计、硬件学习，同时我还重构了内核工程的代码结构。这一部分内容对应的是[汪辰老师慕课](https://www.bilibili.com/video/BV1Q5411w7z5/)中的P20-21，第10章异常和第11章外部设备中断，[github](https://github.com/plctlab/riscv-operating-system-mooc)中的 `code/os/05-traps` `06-interrupts`,以及本项目中的/05/。但愿我不会讲着讲着失去耐心。

### 异常控制流
控制流其实是个很浅显的概念。比如在上一章中我们实现的多任务，task1执行了一会儿，跳转到`task_controller`，`task_controller`作了一些工作，又跳转到了task2.这个流程就叫控制流。这些任务的切换就是正常控制流（我自己起的名字），这个控制流是由`task_controller`控制的。

#### 轮询

但是这种控制流不能覆盖所有的应用场景。比如面对一台windows电脑，你同时看着[汪辰老师的慕课](https://www.bilibili.com/video/BV1Q5411w7z5/)**（task1）**和[github上的代码](https://github.com/plctlab/riscv-operating-system-mooc)**（task2）**，然后突然想在记事本上记点笔记。你每按一下键盘，txt上就显示一个字符，同时两个task还在正常的刷新。如何实现键盘的输入呢？也许你会想到：
```c
void notepad_display(){
  while(1){
    if (键盘输入){
      char c = GetInputFromKeyboard();
      printf(c);
    }
    wait();
  }
}
```
由于我们是协作式多任务，这个函数一点都不协作。导致[汪辰老师的慕课](https://www.bilibili.com/video/BV1Q5411w7z5/)卡住了，根本看不了。于是修改：
```c
void notepad_display(){
  while(1){
    if (键盘输入){
      char c = GetInputFromKeyboard();
      printf(c);
    }
    wait();
    switch_to(&context[task_controller]);
  }
}
```
这样的操作当然能够实现。但会出现一个问题：如果我根本不需要在notepad中记笔记，只想看mooc，刷github。但我的CPU依旧有三分之一的时间被用来进行没有意义的键盘输入检测和空转。这是不是不太好？如果我有十个输入设备，那CPU也别干别的了，一个个输入设备问过去“你有没有要输入的”，就够忙活的了。一个个设备问过去，这就叫做**轮询**。

#### 中断是什么

的确，以目前的知识，轮询只是唯一的办法。为了解决轮询带来的资源占用问题，CPU
接出来了一根中断线`interrupt`，并且指定了一些状态寄存器。回到上面这个问题，键盘也有一根中断线连到了CPU。CPU本来完全不用管键盘，只要专心的播放b站视频，加载代码就行。但一旦我敲了一下键盘，键盘的中断线的高信号就传给了CPU，同时，状态寄存器也会保存这个中断的理由是键盘输入。CPU就放下播放视频的工作，去查询寄存器，发现哦，原来是键盘输入了，于是就跳转到应对键盘输入的入口函数。处理完之后，再回到b站继续播放视频。

有一个关键点，那就是真指令（指令集中的那些指令，不包括伪指令）都是**原子的**，也就是不可被拆分的。不可能一条指令执行到一半，然后去处理中断，再回来执行另一半指令。关于指令的**原子性**之后也会提到。总之，中断一定是一条指令执行完了才去处理他。

#### 异常处理

中断这个机制出现的很早也的确有效。除了应对外设输入之外，类似的机制也被用到了异常处理上。比如我写了一段汇编代码，其中有一句话让我跳转到了一个不存在的地址，比如
```asm
j 0x00000000
```
然后我执行了这段代码。理论上运行之后，我的代码就开始乱转了。但是并没有，CPU内部会检测到，这条指令是非法的，就会告诉我访问了不可访问的地址，然后停掉我的进程。这样的机制就会很好的避免错误指令导致的资源浪费。  
异常处理和能用中断的理论来解释。当你访问了一个内存中不存在的地址，cpu就停下手头的工作，去看看为什么你触发了这个中断，然后调转到相应的异常处理函数中去。

#### 陷阱（trap）
我在上一段讲述的其实云里雾里的，因为我在讲的时候，一会儿说是异常，一会儿说是中断。这是有原因的。因为中断和异常理论上是两种东西，但他们的机制太像了，事实上也共用了一部分的代码和状态寄存器。在这一章里，我会讲中断和异常的关系。

在定义上其实挺好理解，中断是来自外部的。比如键盘中断，鼠标中断。而且中断是程序正常运行的一环。异常就是系统或者用户自己的，比如代码出bug了，访问了不可访问的内存地址，或者访问了无权访问的状态寄存器，就会报异常（Exception）。异常不走中断线，而是CPU自己发现自己异常了，自己跳转到中断处理函数的地址（中断和异常共用一个中断处理函数）。异常就不是正常运行的一环了，最好还是避免他。

但由于中断和异常的实现方法几乎完全类似，包括检测到中断信号后查询的那几个寄存器。我们把他俩统称陷阱（trap）。至于为什么叫陷阱，那是因为他俩都从各自的优先级回到机器模式执行代码，然后再回到各自的优先级。就像陷下去了一样，就叫陷阱。但这个名字好像没那么朗朗上口。不过不重要。

再回到控制流的描述。如果内核从某个正常的进程切换到了陷阱，再回到进程，那我们就叫这个过程异常控制流(Exception control flow)。上面将的都是定义，听起来也许有点莫名其妙。不过不重要，接下来会讲陷阱的软硬件实现。

ps：中断分为外部中断(external interrupts)，定时器中断(timmer interrupts)和软件中断(software interrupts)，在这一章中，我会讲异常和外部中断。定时器中断和软件中断的部分放到下一章。

### 陷阱的硬件实现
这一部分讲所有陷阱共用的硬件实现，也就是一些状态寄存器。这些寄存器会用于各种陷阱的初始化、陷阱的种类查询、陷阱状态查询、陷阱处理函数的跳转等等。这些东西都可以在[c910用户手册的附录](/doc/玄铁C910用户手册.pdf)中查询。

#### mtvec  (machine trap-vector base-address)
*所以他为什么不叫mtvba?*

发生陷阱时，CPU会自动根据这个寄存器跳转到相应的地址。他每位代表的含义如下（这些表都是从[c910用户手册的附录](/doc/玄铁C910用户手册.pdf)中抄的）
63 : 2|1:0  
:---:|:---:  
base|mode  

这里的基地址要求对齐，最后两位都必须是0，所以62位就足够表示64位地址了；mode只有两种，代表两种陷阱函数跳转的模式：
- mode=2'b0 ：所有陷阱跳转到同一个陷阱处理函数，也就是写在base地址的函数。然后陷阱处理函数中再根据陷阱的种类用case语句进行分别处理。**我们用的是这个模式**
- mode=2'b1 ：从base往后依次存放着好几个不同的地址，根据陷阱的类型，CPU会跳转到对应的地址。

强调以下，发生中段后跳转到`mtevc`记录的地址这一步是CPU自动完成的，我们软件上不用写这个跳转。但在内核启动之前需要初始化，也就是自己写陷阱处理函数，然后把地址用`csrw`写到mtevc中。

根据我的实验，再LicheePi启动的过程中，u-boot似乎已经初始化了这个值。不过不重要了，自己写一个陷阱处理函数，自己重新设置`mtevc`就行。

#### mepc  (machine exception program counter)
*这个寄存器的名字又一次证明陷阱、中断、异常这三个概念大家都掌握的模糊不清*

陷阱发生时，CPU会改变`mepc`的值
- 如果发生了异常，`mepc`的值就是造成异常那条指令的地址。
- 如果发生了中断，那就是将要执行的下一条指令的地址。

再次强调，这个寄存器是CPU自己会改变的，我们写内核的时候不用自己去写他。当然，如果在陷阱执行的时候想要改变他，从而再陷阱处理结束后跳转到其他地址，那就另说。

这个寄存器和另一条指令有关：`mret`.`mret`执行的效果是跳转到`mepc`这个地址。同时，恢复CPU在近来之前的状态，包括特权级等。

#### mcause (machine cause)

很直白的含义，记录了陷阱发生的原因。也是陷阱发生后CPU维护的。我们只需要再进入陷阱处理函数后，读这个寄存器，就能判断发生了什么陷阱，然后用case语句处理就行。每一位的含义如下图所是。当然，这张表来源于[c910用户手册的附录](/doc/玄铁C910用户手册.pdf)。
64|63：5|4：0
:---:|:---:|:---:|
interrupt|don't care|exception code  

interrupt位置高代表了这次进入陷阱是因为中断，0代表是因为异常。究竟是什么异常或者什么中断可以参考[c910用户手册](/doc/玄铁C910用户手册.pdf)第五章的表5.1，以下给出一些我们要用到的表项：
中断标记 |异常向量号 |描述
:---:|:---:|:---:
1 |1 |超级用户模式软件中断
1 |3 |机器模式软件中断
1 |5 |超级用户模式计时器中断
1| 7 |机器模式计时器中断
1 |9 |超级用户模式外部中断
1 |11 |机器模式外部中断
0 |2 |非法指令异常
0 |5 |加载指令访问错误异常

#### mtval(machine trap value)
结合`mcause`，能够更加具体的看到异常形成的原因。`mtval`的含义可以参考[c910用户手册](/doc/玄铁C910用户手册.pdf)第五章的表5.2，这里就不再列出了。

#### mstatus(machine status)
这个寄存器是用来修改CPU的一些状态的，其中有一些位置能够用来开启/关闭CPU的中断。[c910用户手册附录](/doc/玄铁C910用户手册.pdf)详细描述了这个寄存器，我把重要的东西放在下表中（这个寄存器有64位，但是下13位对现阶段的我们比较重要，所以只写了这几位）：
12:11|10|9:8|7|6|5|4|3|2|1|0
:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:
MPP|0|SPP|MPIE|0|SPIE|0|MIE|0|SIE|0
- MPP：保存了进入机器模式trap之前的状态。这个寄存器CPU自己保存，`mret`之后会自己恢复到这个特权级
- SPP：保存了进入超级管理员模式trap之前的状态。这个寄存器CPU自己保存，`sret`之后会自己恢复到这个特权级。补充以下，也有进入超级管理员状态的陷阱，但这些东西我们用不到，就不管了。再这个项目中我们只考虑machine特权级
- MPIE：保存了进入机器模式trap之前，陷阱是否使能。`mret`后，CPU会用这个值覆盖`MIE`，因此可以再trap里修改这个值，从而改变trap外面的trap是否使能
- SPIE：略
- MIE：机器模式中段使能位。在进入内核的时候，需要置高，不然之后用不了陷阱
- SIE：略  

我们需要特别关心`MIE`位，这是我们对内核的陷阱初始化需要写的位。

#### mie(machine interrupt enable)

这是第二个我们再进入内核之后需要初始化的地方。`mstatus`的`MIE`位是个笼统的开关，一下子打开所有的中断。但是`MIE`寄存器是细分的开关，可以分别控制外部中断、定时器中断、软件中断的开关。虽然他们都叫`MIE`，但一个是寄存器，一个是寄存器中的一位，不要弄混了。

最后一次说，状态寄存器的详细文档可以参考[c910用户手册](/doc/玄铁C910用户手册.pdf)的附录。`MIE`寄存器每一位的作用如下所示（不展示所有的，只展示关键的[11:0]位）：
11|10|9|8|7|6|5|4|3|2|1|0
:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
MEIE|0|SEIE|0|MTIE|0|STIE|0|MSIE|0|SSIE|0

就不一个一个说了，开头的MS代表了权限是machine还是supervisor；EIE表示external interrupt,TIE表示timer interrupt,SIE表示software interrupt。

### 陷阱的整体流程
#### 初始化
参考[陷阱的硬件实现](#陷阱的硬件实现)， 操作系统内核需要修改三个寄存器`mtvec` `mstatus` `mie`， 设置陷阱处理函数以及陷阱的开关。
#### *执行完某条指令之后要触发陷阱了*
#### top half **（这些事都是硬件作的我们不用操心）**
- 将`MIE` 赋值给`MPIE`， 然后将`MIE`清零。清零是因为我们的规则是：发生trap的里面不能发生trap。
- 设置`MEPC`。如果是异常，就指向上一条指令；如果是中断，就指向下一条指令。
- 根据trap种类设置`mcause` `mtval`  
- 保存当前的权限到`MPP`,然后将当前的权限改成machine。注意：所有trap都必须先回到machine。在处理trap的过程中也许可能会转到supervisor，但在跳出中断之前，绝对不可能转到user。
- 跳转到`mtvec`地址（或者根据`mcause`跳转到`mtvec`之后的某条指令，取决于`mtvec`的最后一位）
#### bottom half **（这些事都是我们写软件内核要做的）**
- 进入`trap_vector`后，第一件事是保存该任务的上下文。`mscratch`依旧保存着进trap那个任务的context地址。这件事情和之前作[多任务](#4-协作式多任务)的步骤一样。
- 根据`mcause`的不同，进入不同的`trap_handler`。例如，外部中断就进入`external_handler`。直接call进去就行。
- 根据不同的陷阱类型，完成操作。如果有必要，可以通过更改`mepc`，使得退出陷阱后进入其他的地方。
- 用`ret`返回到`trap_handler`。注意，只有退出陷阱才用`mret`,我们的陷阱处理还没有结束。
- 根据`mscratch`，加载进入trap之前的通用寄存器。
- `mret`返回任务（这一个指令附带了恢复`MIE`,恢复特权级等操作，不过由于都融合在`mret`中了，所以我们不用管这些）

#### 异常处理的实现
知道了上述的知识，我们就可以实现异常处理了。中断的处理再上面的步骤上还需要其他的支持，因此我们之后再讲。  
异常处理实现的代码在[mooc](https://github.com/plctlab/riscv-operating-system-mooc)中的`/code/os/05-traps`。为了引发异常，汪老师在`trap.c`代码中用了`trap_test`函数
```c
void trap_test(){
	*(int *)0x00000000 = 100;

	uart_puts("Yeah! I'm return back from trap!\n");
}
```
该函数访问了一个不准被访问的地址。因此在执行完这个`ld`指令之后，就出现了异常控制流：之后的`uart_puts`函数暂时被搁置了，CPU跳转到了`mtvec`中定义的
陷阱函数入口地址。参考`trap.c`中的`trap_init`函数，我们将要跳转到`trap.c`中的`trap_vector`函数。
```c
void trap_init(){
	w_mtvec((reg_t)trap_vector);
}
```
`trap_vector`函数在汇编语言`entry.S`中定义。由于我们对`mtevc`中的入口函数地址有要求（记得最后两位必须是0），所以`.balign 4`是必须的。  
这个函数的构造和之前协作式多任务的`switch_to`差不多。就不把代码贴在这了。主要流程是：保存context---call trap_handler---加载context  
不过需要注意`trap_handler`的传参。在调用`trap_handler`之前，我们需要将`mepc` `mcause`传入, 并在执行结束后传出一个新的pc地址
赋值给`mepc`。`mcause`是用来帮助`trap_handler`判断该调用那种陷阱的函数，`mepc`的传入传出就是修改跳出陷阱之后的地址。

`trap_handler`就是一个`switch-case`语句。
```c
reg_t trap_handler(reg_t epc, reg_t cause){
	reg_t return_pc = epc;
	reg_t cause_code = cause & MCAUSE_MASK_ECODE;
	
	if (cause & MCAUSE_MASK_INTERRUPT) {
		/* Asynchronous trap - interrupt */
		switch (cause_code) {
		case 3:
			uart_puts("software interruption!\n");
			break;
		case 7:
			uart_puts("timer interruption!\n");
			break;
		case 11:
			uart_puts("external interruption!\n");
			break;
		default:
			printf("Unknown async exception! Code = %ld\n", cause_code);
			break;
		}
	} else {
		/* Synchronous trap - exception */
		printf("Sync exceptions! Code = %ld\n", cause_code);
		panic("OOPS! What can I do!");
		//return_pc += 4;
	}

	return return_pc;
}
```
`if`就是判断最高位是不是一，是中断还是异常；`cause_code`具体代表什么可以参考前文列出的表格。

### 外部中断的硬件实现-PLIC
这章的后半部分我们要通过外部中断实现串口的输入功能。说白了就是：GROL-OS自顾自的在跑task1，task2.某个时刻，我按以下键盘，通过串口给系统
输入一个字符。这个行为要触发系统的外部中断，把我按的字符打印到屏幕上。然后退出中断，回到应该在的位置，然后继续进行两个任务的轮转。

按照我们之前举的例子，键盘有输入后，interrupt线置高，信号传输到CPU的interrupt口。interrupt只有1bit的位宽。一台点好可能有很多的外设
都需要中断，但是CPU只有一个1bit的interrupt口。而且万一几个外设同时中断，那CPU中断岂不是乱套了。因此需要一个仲裁设备，收集所有外设的
interrupt信号，经过仲裁后，引一根interrupt线链上CPU的中断口。同时，这个设备里面还要有寄存器，记录了当前引发外部中断的设备是谁等信息。
这个设备就叫PLIC(platform-level interrupt controller)

其实plic对每个hart都会引出一条interrupt线。我们依旧只考虑hart0.

显然，PLIC也是一个外设。像UART一样，我们也要知道基地址、各个寄存器的offset以及功能，才能操作他。

#### PLIC base_address
基本上之前找uart0的`base_address`的地方都能找到PLIC的`base_address`。比如[LicheePi4a的uboot的源代码](https://github.com/revyos/thead-u-boot)中，
`drivers/trng/core_rv64.h`文件中定义了如下的宏：
```c
#define PLIC_BASE           (0xFFD8000000ULL) 
```
#### 外部中断ID
在讲述陷阱的时候，我给出了一张表，讲了不同的中断/异常都对应一个`mcause`的值，外部中断的值是7.外部中断也分很多种。比如串口、键盘、鼠标等等。
每一个外设都对应着一个外部中断ID（这个名字也是我自己起的，我好像没注意到这个ID有什么专业的名字）。这个ID会放在PLIC中。某个中断对应的ID是什么，
这个对应关系在[玄铁曳影1520芯片原型用户手册](/doc/玄铁曳影1520芯片原型系统用户手册_v1.0_cn.pdf)的第二章中，表2-3的C910中断映射。
也可以参考[LicheePi4a的uboot的源代码](https://github.com/revyos/thead-u-boot)中的`lib/sec_library/include/soc.h`。
这张表很长，我们只挑选需要的信息：
编号|中断来源|说明
:--:|:--:|:--:
20|int_uart0|UART0中断  

但这还不够！仔细看第18页有句话：
```
注意
E902/C906/C910 0-15 中断保留给内部使用，所以实际的中断号是下面表格中中断编号+16。
```
所以uart0的中断ID是36.这和和qemu的中断ID不一样，要注意改。

C910连接的PLIC最多支持1024个中断ID，其中0号ID不用。

#### PLIC register
C910的PLIC工作原理以及寄存器offset定义在[用户手册](/doc/玄铁C910用户手册.pdf)的8.2章节。也可以参考[LicheePi4a的uboot的源代码](https://github.com/revyos/thead-u-boot)
的`drivers/trng/core_rv64.h`文件中`PLIC_TYPE`结构体的定义。需要注意的是，PLIC中的寄存器都是32位的。以下是关键寄存器的表格：
地址| 名称 |类型 |初始值 |描述
:--:|:--:|:--:|:--:|:--:
0x0|-|-|-|-
0x4|PLIC_PRIO1 |R/W |0x0 |中断源 1 优先级
0x8|PLIC_PRIO2 |R/W |0x0 |中断源 2 优先级
...|
0x0001000 |PLIC_IP0 |R/W |0x0 |1 ～31 号中断中断等待寄存器
0x0001004 |PLIC_IP1 |R/W |0x0 |32 ～63 号中断中断等待寄存器
...|
0x0002000| PLIC_H0_MIE0 |R/W |0x0| 核 0 1 ～31 号机器模式中断使能寄存器
...|
0x01FFFFC |PLIC_PER| R/W |0x0 |PLIC 权限控制寄存器
0x0200000| PLIC_H0_MTH| R/W| 0x0| 核 0 机器模式中断阈值寄存器
0x0200004 |PLIC_H0_MCLAIM|R/W |0x0 |核 0 机器模式中断响应/完成寄存器
- PLIC_PRIOx 表示ID=x的外部中断的优先级。优先级表示同时发生中断，PLIC选择把哪个中断优先交给CPU执行。我们现在不太用管，要用的设置成1就行
- PLIC_IP0 IP表示interrupt pending，读取这个寄存器可以知道哪些外部设备发生了中断。一个寄存器有32个bit，对应的就能表示32个外设有无中断的情况。
- PLIC_H0_MIE是个更加精准的开关。对比之前的`mstatus` `mie`,他能精确控制某个ID的外设中断是否打开。和`PLIC_IP`寄存器一样，一个外设对应一个bit。因此，如果想要打开串口的中断，就要让`PLIC_H0_MIE0[36]=1`
- PLIC-PER和超级用户权限有关，我们就不管他了
- PLIC_H0_MTH引入了一个新的概念叫做阈值。这个阈值是优先级的阈值。也就是当优先级小于阈值的话，PLIC就不把这个中断交给CPU，而是直接扔掉。默认设置为0，也就是允许所有中断。
- PLIC_H0_MCLAIM里面存的是PLIC仲裁后决定交给CPU处理的中断ID，比如串口中断，这里面就放36.当这个中断处理完了之后，就需要把id再一次写进这个寄存器，表示这次中断执行完成。

### 外部中断的代码细节

首先需要初始化。参考`/05/src/plic.c`中的`plic_init`
```c
void plic_init(void)
{
	int hart = r_tp();
  
	/* 
	 * Set priority for UART0.
	 *
	 * Each PLIC interrupt source can be assigned a priority by writing 
	 * to its 32-bit memory-mapped priority register.
	 * The QEMU-virt (the same as FU540-C000) supports 7 levels of priority. 
	 * A priority value of 0 is reserved to mean "never interrupt" and 
	 * effectively disables the interrupt. 
	 * Priority 1 is the lowest active priority, and priority 7 is the highest. 
	 * Ties between global interrupts of the same priority are broken by 
	 * the Interrupt ID; interrupts with the lowest ID have the highest 
	 * effective priority.
	 */
	*(uint32_t*)PLIC_PRIORITY(UART0_IRQ) = 1;
	/*
	 * Enable UART0
	 *
	 * Each global interrupt can be enabled by setting the corresponding 
	 * bit in the enables registers.
	 */
	*(uint32_t*)PLIC_MENABLE(hart, UART0_IRQ)= (1 << (UART0_IRQ % 32));

	/* 
	 * Set priority threshold for UART0.
	 *
	 * PLIC will mask all interrupts of a priority less than or equal to threshold.
	 * Maximum threshold is 7.
	 * For example, a threshold value of zero permits all interrupts with
	 * non-zero priority, whereas a value of 7 masks all interrupts.
	 * Notice, the threshold is global for PLIC, not for each interrupt source.
	 */
	*(uint32_t*)PLIC_MTHRESHOLD(hart) = 0;

	/* enable machine-mode external interrupts. */
	w_mie(r_mie() | MIE_MEIE);

                 
	/* enable machine-mode global interrupts. */
	w_mstatus(r_mstatus() | MSTATUS_MIE);
	
}
```
参考这些宏的定义，以及之前讲的寄存器的含义，能很直接的明白这些都在干什么。

然后是`trap_handler`,考察里面中断相关的case语句
```c
	if (cause & MCAUSE_MASK_INTERRUPT) 
		/* Asynchronous trap - interrupt */
		switch (cause_code) {
		case 3:
			uart_puts("software interruption!\n\r");
			break;
		case 7:
			uart_puts("timer interruption!\n\r");
			break;
		case 11:
			#ifdef MYPRINT
			uart_puts("external interruption!\n\r");
			#endif
			external_interrupt_handler();
			break;
		default:
			printf("Unknown async exception! Code = %ld\n\r", cause_code);
			break;
		}
```
注意：这个宏`MCAUSE_MASK_INTERRUPT`需要修改，mooc中是32位机器，因此是`0x80000000`
在改称64位之后，需要改称`0x8000000000000000`

在`case 11:`的时候，跳转到了`external_interrupt_handler()`，参考之前的陷阱映射表，11就对应着外部中断。

接下来看`external_interrupt_handler`
```c
void external_interrupt_handler(){
	int irq = plic_claim();

	if (irq == UART0_IRQ){
		#ifdef MYPRINT
		printf("uart0 interrupt irq = %d\n\r", irq);
		#endif
      	uart_isr();
	} else if (irq) {
		#ifdef MYPRINT
		printf("unexpected interrupt irq = %d\n\r", irq);
		#endif
	}
	
	if (irq) {
		plic_complete(irq);
	}
}
```
这段代码的首尾都涉及到了`plic_claim/plic_complete`寄存器，在中断开始的时候读中断ID，在中断结束的时候原样给他写回去。

`UART0_IRQ`宏就是之前说到的从文档里查出来的串口中断号36.中断处理函数`uart_isr`就是从串口寄存器中读输近来的字符，然后`printf`,不再赘述。

到这儿就可以跑通了，我们终于实现了上一章中的目标：一边刷b站，一边刷github，同时往txt里写笔记。

其实我在作这块内容的时候出了很多bug，我明明没有按键盘，却疯狂的触发串口输入中断。莫名其妙。
最终凑出了一个奇怪的解决办法：在uart中断是能之前读取uart的USR寄存器。不知道为什么他就好了。
接下来又遇到问题：系统刚启动就触发了一次中断。好在这次中断的串口ISR寄存器和正常中断的不一样。所以我在`uart_isr()`函数中用一个if语句排除了这个中断。
```c
//void uart_isr()
if ((isr & ISR_IID) == 1) return;
```
### 一些杂活

#### 重构项目
主要是因为mooc的项目太乱了，一个文件夹下有头文件源文件连接器脚本。所以我把代码分类，
脚本文件夹放链接器脚本，src文件夹放.c文件，里面的asm文件夹放.S汇编语言，include里面放头文件
build里面是生成的中间文件，bin就是最后要放到板卡离去的可执行文件。

这部分内容需要进一步的学习makefile脚本的语法，参考[这个网站](https://blog.csdn.net/linuxweiyh/article/details/90301424)学习就行。修改后的
代码参考`05/Makefile`以及`common.mk`

#### 让串口输出变得好看
之前的串口输出都是哗啦啦的向下滚。我想实现的效果是，task1的输出等一会儿，然后消失，变成task2，然后换换颜色，像跑马灯一样，好看一点。

同时，最后一行显示的是GROL-OS的命令行，就像bash一样，我把他叫做`goish`,并设计`goish`的第一条指令`WhoAmI`,能让串口输出当前版本。

需要用到的知识就是如何让串口移动光标，如何改变串口输出的颜色。具体操作参考[这个链接](https://blog.csdn.net/lzuacm/article/details/8993785)如下：
- `\r` 让光标移动到该行行首
- `\n` 让光标移动到下一行
- `\033[nA` 让光标上移n行
- `\033[K` 清除该行
- `\008` Backspace

这个`\033[`用处很大。[这个链接](https://blog.csdn.net/lzuacm/article/details/8993785)有详细的改变输出字体颜色的教程。

我结合了这些技巧，第一次给GROL-OS设计了一个简单的GUI界面。在05目录下执行`make`命令就行了。输入的字符会出现在goish中，并且输入`WhoAmI`会输出当前的版本。

到此，05项目结束。不过这个GUI由于是临时起义写的，很草率，复用性不高，需要每一个task自己写串口输出的排版。很不好。之后打算写一个屏幕刷新的进程，负责从各个task中调取该输出的内容，调度屏幕内容，能够很好的显示在屏幕上。之后再说。

最后说一下，如果仅仅是让系统打印debug信息，在我的工程里，最好用`make myprint`来make，因为后来我由于不维护`make`,输出的内容太乱了，不好debug。

## 6. 计时器中断与软件中断

这一部分内容对应的是[汪辰老师慕课](https://www.bilibili.com/video/BV1Q5411w7z5/)中的P20-21，第12章和第13章，[github](https://github.com/plctlab/riscv-operating-system-mooc)中的 `code/os/07-hwtimer` `code/os/06-preemptive`,以及本项目中的/06,/07。


我们之前提到过抢占式多任务。说系统时不时就要把控制权切一下。这个时不时就说明我们需要一个计时器来产生中断。我们脑补以下这个流程：
- task1跑着跑着，时间差不多了，一个叫做计时器的外设给cpu发送了一个中断
- 这个中断的处理过程就是把任务从task1切换到`task_controller`
- 然后`task_controller`自己把控制权交给下一个任务，算了以下，得交给`task2`
- 重复上述工作。

我们可以看到，在这个过程中，我们采用了两种任务切换的方式。一种是依靠类似外设中断的方式，有个定时器给CPU发送了中断，然后再`trap_handler`中，完成了任务的跳转。
另一种是依靠类似协作式多任务的方式：`task_controller`自己不干了，挑选了一个任务，把执行权交给他。

对于第二种任务切换的方式，我们当然可以沿用协作式多任务的方法来完成任务的切换。但是会有一个问题：我们的内核中有两种不同的多任务机制。这不好。强迫症犯了。同一件事情为什么要用两种办法来做呢？

那能不能这样：`task_controller`自己叫一个中断，这样就能进入`trap_vector`，然后在陷阱处理的时候，存储、加载上下文，切换返回地址，完成任务的切换。这种设想非常的整齐，非常的美妙。

所以为了实现抢占式多任务，我们就引入了两种中断：让task1到时间了切换到`task_controller`的定时器中断，以及`task_controller`自己配置好了以后移交执行权的软件中断。

当然这两个中断可以感的事情不仅仅是这些。其他的作用等要用了再说。总之，这一章的任务就是实现抢占式多任务。

### 这两种中断的硬件实现(CLINT)

不管是qemu还是licheepi，都采用了`CLINT`(Core local interrupter)来实现定时器中断和软件中断。 

#### CLINT base_address
参考[LicheePi4a的uboot的源代码。](https://github.com/revyos/thead-u-boot)
```c
#define CORET_BASE          (PLIC_BASE + 0x4000000UL)                 
/*!< CORET Base Address */
#define CLINT_BASE          (PLIC_BASE + 0x4000000UL)                 
/*!< CLINT Base Address */
```
计算后就知道他的基地址是`0xFF_EC00_0000`
#### CLINT register
LicheePi的CLINT寄存器offset和qemu的很不一样。但是作用差不多。参考[用户手册](/doc/玄铁C910用户手册.pdf)的8.1章。我把寄存器表的重要内容放在下面。
地址 |名称 |类型 |初始值| 描述
:--:|:--:|:--:|:--:|:--:|
0x0 |MSIP0 |读/写 |0x00000000 |核 0 机器模式软件中断开关，bit[0] 有效
..|
0x4000 |MTIMECMPL0 |读/写 |0xFFFFFFFF| 核 0 机器模式时钟计时器比较值寄存器 (低 32 位)
0x4004 |MTIMECMPH0 |读/写 |0xFFFFFFFF| 核 0 机器模式时钟计时器比较值寄存器 (高 32 位)

- MSIP0:开关。想要打开软件中断，就往这个地方写1.注意：即使其中段没有着一个级别的开关。只要把前面的`mcause` `mie`开了就行。
- MTIMECMPL:这个东西的使用涉及到计算机原理的知识。和PLIC一样，CLINT的寄存器也是32位。在rom或者u-boot中，我们把这些寄存器映射到了内存空间里去。他们的地址`0x4000` `0x4004`看起来是连续的，
其实并不是。比如我用一个`uint64_t *`类型的指针去读`0x4000`地址，得到的并不是高32位和低32位的拼接，而是两个低32位的拼接。所以读写这俩寄存器的时候一定要注意。我就踩了这个坑。

这里给出设置`MTIMECMPL`的函数代码，注意，一定要两块分开读写。
```c
*(uint32_t*)CLINT_MTIMECMPL(id) = (uint32_t)(MTIME & 0xffffffff) ;
*(uint32_t*)CLINT_MTIMECMPH(id) = (uint32_t)((MTIME >> 32) & 0xffffffff) ;
```

除此之外，还有一个关键寄存器`time`,这是一个`user`权限的控制状态寄存器。他是用来计时的，刚开机的时候是0，每过以小段时间就加一。可以用csrr来读他。

计时器中断触发的原理是：每当`time`大于等于`CLINT_MTIMECMP`,就触发中断。为了避免持续触发中断，每次计时器中断都要重新设置`CLINT_MTIMECMP`的值。这样就能让计时器周期性的产生中断，从而能够很好的完成抢占式任务。

同时参考老师的课后练习，我需要一个每秒加一的系统时钟。但我是在是翻遍了文档也找不到time增长的频率。最后经过测试，感觉大概每过一秒，time寄存器就增加3000000.这个数字凑合这用把，每分钟的误差不超过1s。

软件中断就不多说了，每次要中断的时候，只要写`CLINT`的`MSIP0`就行了。记得中断处理完后把`MSIP0`写回去，不然就一直在软中断了。

### 抢占式多任务的流程
- 触发定时器中断，进入`trap_vector`,保存了上下文之后，把`mepc` `mcause` 传入`trap_handler`
- 定时器中断是7号中断，进入case语句后进入`timer_handler`.此时还在陷阱里。
- `timer_handler`改变`CLINT_MTIMECMPL`的值后，判断是否满足切换任务的条件。如果满足，就利用`switch_to`函数切换到`task_controller`.注意，`switch_to`函数需要修改。
  ```asm
  switch_to:

    csrw	mscratch, a0


    LOAD	a1, 31*SIZE_REG(a0)
    csrw	mepc, a1
    #read mepc of context and set it


    # Restore all GP registers
    # Use t6 to point to the context of the new task
    mv	t6, a0
    reg_restore t6

    # Do actual context switching.
    mret

  .end
  ```
  主要有三处改变：  
  1. 不用再保存上下文了，因为上下文在进入陷阱的`trap_vector`中就保存过了
  2. 需要修改`mepc`,因为最后需要用`mret`退出陷阱，而退出陷阱回到的地址不是`ra`而是`mepc`
  所以需要修改`context`结构体，在结构体中保存`mepc`
- 注意，这个时候已经退出陷阱了，在正常的任务流中。
- `task_controller`根据上一个任务的编号、优先级等，找到下一个需要执行的任务。并通过`call_software_interrupt`函数跳转到下一个需要执行的任务。
  ```c
  void call_software_interrupt(uint8_t flag, uint64_t arg){
    software_interrupt_flag = flag;
    software_interrupt_arg = arg;
    reg_t hartid = r_mhartid();
    w_msip(hartid, 1);
  }
  ```
  在这段代码中，`software_interrupt_flag` `software_interrupt_arg`是用来记录中断到底是哪种中断以及中断的参数。比如现在这个情况，`task_controller`就要向软件中断传递下一个任务的上下文地址。
- 然后就进入了中断控制流。同样是`trap_vector`- `trap_handler`的流程，然后通过case语句进入了`software_handler`
- 在软件中断中，通过刚才修改的`switch_to`函数就能跳转到对应的任务，同时跳出陷阱。

至此，抢占式多任务的流程就算完成了。

### 额外工作
- 实现任务的优先级功能。高优先级的任务执行完了才能执行低优先级的任务。`uint32_t priority`变量在之前已经添加到`context`结构体中了，现在只需要修改`task_controller`就行了，这部分不太难，看代码就行。

### 关于c中的static关键字
有些重要的变量，最好通过static关键字定义。这样，这个变量只能在这个文件里调用。如果其他文件想要调用，那就写一个函数，文件外调用就用这个函数调用就行。

依旧懒得写UI。之后再说。

## 7.任务同步与锁

这一章东西不多，轻松一下。

### 并发

并发的意思就是：两个任务要访问同一块内存区域。如果task1并没有完成全部任务，`task_controller`就停止了`task1`的工作，让`task2`来做，那就会出错，执行效果和预期不一样。毕竟我们执行多任务的时候，目标是就和每个任务单独执行的结果一样。

比如说我有下面这两个任务：
```c
static int i;
void task1(){
  for (i=0; i < 5; i++) printf("Im task1\n\r");
  printf("task1 finish\n\r");
}

void task2(){
  for (i=0; i > -5; i++) printf("Im task2\n\r");
  printf("task2 finish\n\r");
}
```
如果只运行task1,或者任务2，他们的输出结果是
```
Im task1
Im task1
Im task1
Im task1
Im task1
task1 finish
```
放在多任务机制里，我们期待的结果也应该是这样的（不一定刚好交错。也可能是task1输出3次，task2输出3次）
```
Im task1
Im task2
Im task1
Im task2
Im task1
Im task2
Im task1
Im task2
Im task1
Im task2
task1 finish
task2 finish
```
但事实并不是这样。如果用我们之前提到的多任务方式，采取一个合适的间隔切换多任务，输出结果会在两个for语句中一直循环，始终不会finish。
```
Im task1
Im task2
Im task1
Im task2
Im task1
Im task2
Im task1
Im task2
Im task1
Im task2
...没完没了
```
比如首先是task1，在执行到i=2之后，切换到了task2，i就被初始化成0了。i执行到-2后，又切换到task1，执行到i=0后，有切换到task2....就没完没了了。

我们总不能在切换多任务的时候，把共用部分的内存也都备份一下吧，这工作量可太大了。

总之，这种问题就叫做并发，解决这种问题的方法就叫做同步。

还有一些概念，比如临界区，就是上面这个例子里的`static int i`，就是两个任务共用的一块内存区域。

### 锁
锁就是解决并发问题的一种办法。对于每一块临界区，我们都设置一把锁。拿了锁的才能读写这块区域，没拿锁的就卡在这儿等着，知道锁被还回来。运用这个概念，我再重写一下上面这两个例子：
```c
#define LOCK_FREE flase
#define LOCK_BUSY true
static int i;
static bool lock = LOCK_FREE;

void task1(){
  while (lock == LOCK_BUSY) keep_wait();
  lock = LOCK_BUSY;
  //acquire lock
  for (i=0; i < 5; i++) printf("Im task1\n\r");
  printf("task1 finish\n\r");
  //work
  lock = LOCK_FREE;
  //release lock
}

void task2(){
  while (lock == LOCK_BUSY) keep_wait();
  lock = LOCK_BUSY;
  //acquire lock
  for (i=0; i > -5; i++) printf("Im task2\n\r");
  printf("task2 finish\n\r");
  //work
  lock = LOCK_FREE;
  //release lock
}
```
依旧是一些名词定义：想要获取执行权的操作叫做`acquire`lock;想要释放执行权的操作叫`release`lock。像我这里写的一样，如果没有等到锁，就要一直空转的锁的实现叫做自旋锁`spin lock`，也叫`非睡眠锁`；没有等到锁就直接跳到下一个进程的锁叫做`睡眠锁`。  

但是这种写法还是**有问题**的！
我们看到，acquire操作基本需要两句话才能完成的。一句是`while`语句，是用来实现锁的检测。一句是赋值语句，用来实现锁的赋值。如果在任务一中，锁通过了检测，但是还没等到赋值就切换任务了，那么任务二也能通过检测。这样的话，两个任务就都拥有了锁，那就还是没有解决并发的问题。

解决方法涉及到CPU的指令集。我们之前提到过指令的原子性。事实上，riscv有一类指令A指令，里面放的都是原子性的指令，也就是能同时执行上述的检测锁、赋值锁两个操作而不被打断。我们要用到的是这个指令。参考[用户手册](/doc/玄铁C910用户手册.pdf)14.3附录A-3.
```asm
amoswap.d.aq   rd, rs, (addr)
#将（addr)地址的内存赋值给rd，将rs赋值给(addr)地址的内存。d代表64位，也可以换成w，代表低32位   
```
其实这个指令没我讲的那么简单。最后的.aq也是有含义的。aq表示这条指令执行完之前，其他访问(addr)的指令都将被阻塞。rl表示其他访问(addr)的指令结束之前，该指令被阻塞。

也可以简单理解为，.aq是后原子，他后面的指令不会打断他；.rl是前原子，他前面的指令不会打断他。也可以写.aqrl，那就是全原子，前后的都不能打断。

通过这个指令，我们可以这样实现锁的获取：
```c
#define LOCK_FREE flase
#define LOCK_BUSY true
static bool lock = LOCK_FREE;

void mutex_acquire(){
  bool tmp_lock = LOCK_BUSY;
  do{amoswap.w.aq tmp_lock, tmp_lock, &lock;
  }while (tmp_lock == LOCK_BUSY);
}
```
可以仔细想象实现的效果时不时和我们预期的一样：如果锁是空闲的，就跳出了；如果锁被占用，就会一直困在while中。


关于锁的多种实现方式，可以阅读亚伯拉罕的《操作系统概念》第六章。书中提到一个概念叫做 **“有限等待要求”**。我们新构建以下两个任务：
```c
void task1(){
  while(1){
    mutex_acquire();
    //acquire lock
    do_something();
    //work
    mutex_release();
  }
}

void task2(){
  mutex_acquire();
  //acquire lock
  do_something();
  //work
  mutex_release();
}
```
最关键的地方是任务一是在死循环中的。假设每次切换任务的时候，任务一都在`do_something()`中，那任务二就永远也不会被分配到锁。这就是不满足有限等待要求。书中给出了一种方法叫做 **信号量**，感兴趣的同学可以实现一下。

我没有用信号量来实现。我实现的方式很草率：一旦有锁的释放，我就切换任务。同样，一旦没有获取到锁，我也切换人物。这样的操作虽然能够满足 **有限等待要求**，但是当任务频繁的调用锁，且每次很快就还锁时，这个人物就跑不满一个相对公平的时间。不过我们对每个任务的公平性还没有很高的要求。就这样吧。

同样的，这一章内容也没有升级UI。跑的时候用make凑合着就行。

## 8.软件定时器与系统调用
### 软件定时器和硬件定时器
什么是硬件计时器？我们可以查看[文档](/doc/玄铁曳影1520芯片原型系统用户手册_v1.0_cn.pdf)中的中断映射表。其中有`Timer1中断`的字样。这个就是硬件计时器。他是连在`PLIC`上的，通过外部中断实现的计时。这部分内容老师的可上没多讲。但我们可以想象到，这样的定时器数量是有限的，取决于硬件中`timer`的个数。但显然他的计时更加准确。

之前我们讲述过定时器中断。定时器中断的核心是`mtime`寄存器，他一直在自增，同时和`CLINT`中的比较寄存器进行比较。我们可以在这个基础上构建软件定时器。他更加灵活，药用就可以构建一个。而且不需要额外的知识就可以编写，只需要熟练调用CLINT计时器就行。这部分可以看作一个代码编写的训练。在这部分我给自己定的目标是：自己写软件定时器，同时将抢占式多任务改成建立在软件定时器的基础上的。
### 软件定时器的代码实现
软件定时器的结构如下所示，定义在`/include/timer.h`中：
```c
struct swtimer{
    void (*func)(void *arg);
    void *arg;
    uint32_t timeout_tick;
    uint32_t period;
    uint8_t flag;
    //a tick equals to (1 / CLINT_SLICE_PER_SEC) second
};
```
第一句话中的`func`函数就是软件定时器时间到了之后需要调用的函数。

`timeout_tick`就是下一个触发软件定时器的时间点。

`flag`表示这个定时器是一次性的还是周期性的。根据`flag`不同，`period`的含义也不同，分别代表多长时间后执行函数，或者执行函数的周期。

在新建一个软件定时器之后，我们需要将这个结构体重的内容按照需要初始化。初始化函数在`src/timer.c`中，没什么好说的。
```c
struct swtimer *swtimer_create(
    void (*handler)(void *arg),
    void *arg,
    uint32_t timeout,
    uint8_t flag
){
    int avalible_timer;
    for (avalible_timer=0; avalible_timer < MAX_SWTIMER; avalible_timer++)
        if (SWTIMER_NOT_EXIST == _swtimer[avalible_timer].flag) break;
    _swtimer[avalible_timer].func = handler;
    _swtimer[avalible_timer].arg = arg;
    _swtimer[avalible_timer].timeout_tick = timeout + _tick;
    _swtimer[avalible_timer].flag = flag;
	_swtimer[avalible_timer].period = timeout;
	#ifndef myprint
	printf("swtimer created.id=%d, current_tick=%d, next_tick=%d\n\r", avalible_timer, _tick, _swtimer[avalible_timer].timeout_tick);
	#endif
}
```

判断函数是否触发是放在定时器中断中的，每次产生定时器中断都需要检查`_tick`和`swtimer.timeout_tick`的值，然后执行函数，并且根据`flag`的值，删掉这个计时器，或者更新`timeout_tick`.
```c
void swtimer_check(){
	for (int i=0; i < MAX_SWTIMER; i++){
		if ((_swtimer[i].timeout_tick <= _tick) && (_swtimer[i].flag != SWTIMER_NOT_EXIST)){
			#ifdef MYPRINT
			printf("swtimer:id=0,current_tick=%d, timeouttick=%d\n\r",  _tick, _swtimer[0].timeout_tick);
			#endif
			if (_swtimer[i].flag == SWTIMER_DISPONSIBLE) _swtimer[i].flag = SWTIMER_NOT_EXIST;
			_swtimer[i].timeout_tick = _swtimer[i].period + _tick;
			w_mstatus_MPP(MSTATUS_MPP_MACHINE);
			_swtimer[i].func(_swtimer[i].arg);
		}
	}
}
```

这部分中我产生了一个疑问，那就是`func`函数是在`machine`权限调用的，但是却可以让用户编写。这样会不会产生安全问题？答案显然是会的。这部分内容我们放到系统调用部分去解决。因为到了系统调用那部分我们才会讲如何切换特权级。

回顾一下我们一开始的目的：用软件定时器实现抢占式多任务。因此，我们在`task_controller`创建的时候，设置一个与他对应的软件计时器，他的创建函数如下：
```c
			_task_controller_timer = swtimer_create(schedule, 0, TICKS_PER_SLICE, SWTIMER_PERIODIC);
```
这个`schedule`函数就是一个`switch_to`函数，跳转到`task_controller`就行。这部分的内容很简单，就告一段落了。

### 系统调用
这部分内容的可玩性很高。结合系统调用，可以解决之前遇到的一系列问题。

#### 权限问题
我们之前提到过，riscv有三个权限级：M(machine), S(supervisor), U(user)。CPU上电之后工作在M权限级。随之而来的是两个问题：CPU如何实现权限级别的切换？U权限如何访问一些M权限的寄存器？这两个问题其实是同一个问题，因为user访问machine的状态寄存器的方式就是切换到machine权限，读取寄存器，再返回user权限的。而trap就是一种很直观的权限切换的方法。由此我们引入了系统调用。

系统调用和软中断很像，都是user主动切换到machine去干一些活儿。但是软中断是中断，系统调用却是一种异常处理。可以参考[文档](/doc/玄铁C910用户手册.pdf)的表5-1异常和中断向量分配表，系统调用是8号异常，也叫做用户模式环境调用异常。既然是异常，`mepc`值就是发生异常的那行代码。因此我们需要手动给`mepc`值加上4.

#### 汇编中系统调用的代码实现
rv的汇编提供了两个指令来实现系统调用 `ecall` 和`eret` 。`ecall`可以参考`w_sip`指令，可以直接触发一个异常。系统调用的参数传递可以用寄存器来传递，也就是把需要传递的参数放在`a7`， 然后在`trap_handler`中通过上下文访问这两个寄存器的值，就可以实现参数传递了。相关代码节选如下图所示：
```c
#define SYSCALL_KILL_MYSELF         0
#define SYSCALL_GET_MTIME           1
#define SYSCALL_JUST_YIELD          2
#define SYSCALL_GET_MSCRATCH        3
#define SYSCALL_SEMAPHORE_WAIT      4
#define SYSCALL_SEMAPHORE_SIGNAL    5
extern uint64_t user_syscall(uint64_t syscall_type);//call this function in user mode
```
```asm
user_syscall:
    addi    a7, a0, 0
    ecall
    ret
```
上面两段代码都是系统调用之前，还在user模式下。下面这段代码就是异常处理中对系统调用的处理。每一个case都是对应的系统调用类型。
```c

static void syscall_handler(struct context * syscall_context){
	printf("syscall_code:%d\n\r", syscall_context -> a7);
	switch (syscall_context -> a7) {
	case SYSCALL_KILL_MYSELF :
		#ifdef MYPRINT
		printf("ask for kill itself\n\r");
		#endif
		task_num_sub();
		syscall_context -> flags = 0;
		newpriority();
		w_mstatus_MPP(MSTATUS_MPP_MACHINE);
		schedule();		
		break;
	case SYSCALL_GET_MTIME:
		#ifdef MYPRINT
		printf("ask for mtime\n\r");
		#endif
		syscall_context -> a0 = r_time();
		break;
	case SYSCALL_JUST_YIELD:
		#ifdef MYPRINT
		printf("ask for yield\n\r");
		#endif
		
		w_mstatus_MPP(MSTATUS_MPP_MACHINE);
		schedule();		
		break;
	case SYSCALL_GET_MSCRATCH:
		#ifdef MYPRINT
		printf("ask for mscratch:%p\n\r", r_mscratch());
		#endif
		syscall_context -> a0 = r_mscratch();
		break;
	default:
		panic("UNKNOWN SYSCALL_CODE\n\r");
		break;
	}
}

```
这时候就会发现：系统调用的功能和软中断几乎一样。那他们的区别在哪里呢？我问了chatgpt：
- 软中断通常设计用于处理异步事件和任务调度，而不是频繁的系统调用。如果用软中断来实现系统调用，可能会导致以下性能问题：
开销增加：软中断的处理机制通常比ecall更复杂，可能需要更多的处理时间和资源。
响应时间：系统调用通常要求快速响应，而软中断可能会受到其他中断或调度延迟的影响，导致系统调用响应时间增加。
- 安全性问题
系统调用通常涉及用户态和内核态的切换，需要严格的权限控制和安全检查。使用软中断可能会引入安全漏洞：
权限管理：软中断处理程序需要正确管理权限和安全上下文，否则可能导致特权级别提升的漏洞。
输入验证：系统调用参数需要严格验证，软中断处理程序可能缺乏这方面的安全措施，容易引入错误和安全漏洞。
- 复杂性问题
系统调用机制已经在操作系统中被广泛优化和标准化，使用软中断实现系统调用会增加系统的复杂性：
代码复杂性：使用软中断实现系统调用需要额外的代码来设置和处理中断，增加了系统代码的复杂性。
维护成本：不同的软中断处理程序需要维护和调试，增加了操作系统维护的工作量。

总之，我的出的结论是：上下文切换部分用软中断来完成，其他的只是进去一下获取寄存器的值，用系统调用来完成。

当然还有一个区别：软中断只有S和M模式能调用。如果想要在U模式下访问M态，只能用系统调用。

#### 通过系统调用实现一些新的功能

##### 软件定时器的安全性问题（未完成）
之前提到过，软件定时器会让用户写一段机器模式下执行的代码，会带来安全性的问题。想要解决之个问题，我们应该让即使其中段之后顺延一部分在user模式下执行的部分，然后再返回上下文，由此带来一个新的问题：如何让定时器中断能有一部分在`user`模式下执行呢？  
想到的解决方案是：在`swtimer_check`函数中，判断需要执行的函数是用户的还是系统的。如果是系统的，就按照上文所述，执行完之后按照顺序`mret`就行。  
那如果是`user`的函数，就切换权限并`mret`到一个新函数中，这个新函数会调用`func`函数，并在执行完之后返回原始函数。

新的问题：调用`func`函数的栈在哪儿？我想了想，也许可以借用原始函数的上下文。应该可以，执行完之后不保存，返回原函数后还是原来的上下文。  
- 问了一下`chatgpt`，其实中断的栈本身就有很多中处理方法。GROL-OS中trap用的是上下文本身的栈-因为我们在调用handler函数的时候并没有改变sp值。最后load上下文或者switch的时候会自动抹掉这部分栈。有的会专门给上下文开一个栈。应该阿也很简单，只要在保存上下文之后给sp赋值一下就行。现在暂时也没必要。

第二个问题：新函数如何返回原始函数。这块应该需要写一个`user`切换到`user`上下文的`switch_to`函数了。这一部分就是之前协作式多任务的`switch_to`

最后一个问题：如何判断一个函数是否是机器状态下执行的。我想了想，也许可以构建一个函数白名单，里面有上下文切换这类的函数。这些函数在机器模式下执行，其他的在用户模式下执行。还是这个办法最简单。

但是这个方案还是不行。从user模式跳转到原始上下文，在汇编下是实现不了的。因为如果要跳转到原始的上下文，我们就需要让某个寄存器保存原始上下文的地址。但是我们加载上下文后，所有的寄存器都满了，没地方塞这个地址。我尝试了好几次后失败了，但是代码的接口还在。可以参考`src/timer.h`中`timer_check()`中的ifelse语句
```c
  if (_swtimer[i].func == schedule){
    #ifdef MYPRINT
    printf("swtimer call machine func!!!\n\r");
    #endif
    w_mstatus_MPP(MSTATUS_MPP_MACHINE);
    _swtimer[i].func(_swtimer[i].arg);
  }
  else{
    #ifdef MYPRINT
    printf("swtimer call user func!!!\n\r");
    #endif
    w_mstatus_MPP(MSTATUS_MPP_USER);
    swtimer_user_first_stage(_swtimer[i].func, _swtimer[i].arg);

  }
```
else中的`swtimer_user_fist_stage`函数在`src/asm/swtimer.S`中。反正最后是跑不起来。我不管了。

另：课程群中有同学这么解释，说这个安全性问题不用在意：
- M/U模式一般都是嵌入式使用的，嵌入式一般程序和GUI固定的，不需要考虑安全性，因为用户接触不到系统调用、编码啥的。一般用户只能操作GUI(比如:自动贩卖机)非要解决的话，U态运行+PMP。当然这只是相对来说安全些
- 除非支持M/S/U三模式和分页。这样用户没办法操作具体物理内存和定时器相关寄存器了

在这里感谢`@Mo Huacong`的帮忙。

![主页](/mdpic/7.jpg)

再立个flag，这个项目到最后需要实现三模式以及分页。

##### 锁的优化
回顾一下上一章中锁的实现：
```c
struct mutex{
  bool available;
};//defination of mutex

void acquire(){
  while (!available)
    switch_to(anotherTask);
  available = false;
}

void release(){
  available = true;
  switch_to(anotherTask);
}
```
再这种mutex的实现上，为了能够有限等待，我们加入了两个`switch_to`函数用于上下文切换。
- `acquire`中的`switch_to`是为了避免没获得锁的任务只能忙转等待，浪费时间的情况。
- `release`中的`switch_to`是为了避免有锁的任务一直占用着锁，导致其他任务无限等待的情况。

但是这种实现方法也会带来问题：
- 如果一个任务没有锁，`task_controller`每一轮都要判断一下有没有锁然后退出，任务切换是很大的开销。我们应该让等不到锁的任务直接退出`task_controller`的就绪队列，直到锁被释放出来，才执行这个任务。
- 如果一个任务申请了锁，然后很快就释放了锁，那么这个就不得不让出自己的控制权（因为`release`中的`switch_to`）。这是很不公平的。但如果让他继续执行到再次获得这把锁，就对其他再等待这把锁的任务不公平了，没有实现有限等待。因此需要像一个办法，有满足有限等待，有要让`release`的时候不切换任务。

因此我们提出了**信号量**

信号量的软件实现如下所示：
```c
struct semaphore{
  int value = 1;
  struct process list[LIST_SIZE];
};

void wait(struct semaphore *S){
  S->value--;
  if (S->value <= 0){
    add this process to S->list;
    block_this_process();
  }
}

void signal(struct semaphore *S){
  S->value++;
  if (S->value < 0){
    remove the first process P from S->list;
    wakeup_process(P);
  }
}
//注意：这两段伪代码中，对S->value的++与判断都是原子操作，同时进行的。
```
信号量的实现可能没有mutex这么直观。但流程上和mutex区别还是不大的。`wait()`就是mutex中的`acquire()`，`signal()`就是mutex中的`release()`.难以理解的就是这个`value`到底是什么，建议还是盯着这两段代码在脑子里过一遍。不过信号量的学习中有一个抓手，就是**value如果是负数，那么他的绝对值就是正在等待这把锁的进程数目**。

所以`wait`中对`S -> value`的--与判断的意思就是：如果我申请这把锁之后，这把锁就有大于一个进程在等待了，那么就说明我这个进程也是需要等待的。那么我就把自己放到这把锁的等待队列中去，然后把我这个进程`block`了，并切换到下个进程。直到这把锁该轮到我用了，不然`task_controller`是不会切换到我的。  
所以`signal`中对`S -> value`的++与判断的意思就是：我释放了这把锁，如果这个时候还有进程在等着把锁，我就把第一个来排队的进程给解封了，并且把他从等待列表中移除。做完这些后并不用交出控制权，因为哪怕之后这个任务再次申请这把锁也申请不到。

注意到这个`S -> list`是一个先进先出的队列。由于我没有实现malloc，所以依旧用数组的方式来维护他。由于需要有原子操作，因此这些代码需要用汇编来写。我写的时候de了两天bug，很折磨。这部分的汇编代码在`/src/asm/semaphore.S`中.
```asm

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


```


写这部分汇编代码的时候需要注意：
- struct的地址偏移问题。建议写的时候printf一下需要用的变量和struct头之间的偏移值。这个东西很玄学，应该取决于编译器怎么分配地址。我写的时候一会儿变量排的紧紧的，一会儿又有64位的对其。我懒得学，printf一下能解决所有问题。并且如果结构体最后的一个变量是个`uint8_t`，用`ld`命令修改他，会修改到他后面的变量。为了方便，建议所有变量都`uint64_t`
- sp指针的问题。要修改函数栈大小的时候，一定要将函数头、函数尾的三个地方的函数栈大小修改一下。这个问题也很难debug。
  
  
**计划更改：下一步打算把这些东西全部移植到FPGA上。我手头有一个rv32的ip，可以在这个的基础上加上clint，plic等，试着把这个操作系统boot上去。**
