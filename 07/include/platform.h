#ifndef __PLATFORM_H__
#define __PLATFORM_H__

/*
 * QEMU RISC-V Virt machine with 16550a UART and VirtIO MMIO
 */

/*
 * maximum number of CPUs
 * see https://github.com/qemu/qemu/blob/master/include/hw/riscv/virt.h
 * #define VIRT_CPUS_MAX 8
 */
#define MAXNUM_CPU 8

/* used in os.ld */
#define LENGTH_RAM 0xfdfffff

/*
 * MemoryMap
 * see https://github.com/qemu/qemu/blob/master/hw/riscv/virt.c, virt_memmap[]
 * 0x00001000 -- boot ROM, provided by qemu
 * 0x02000000 -- CLINT
 * 0x0C000000 -- PLIC
 * 0x10000000 -- UART0
 * 0x10001000 -- virtio disk
 * 0x80000000 -- boot ROM jumps here in machine mode, where we load our kernel
 */

/* This machine puts UART registers here in physical memory. */
#define UART0 0xffe7014000L


/*
 * INTERRUPT related
 */
#define UART0_IRQ 36

//codes below related with the code of u-boot
#define PLIC_BASE 0xffd8000000L
#define PLIC_PRIORITY(id) (PLIC_BASE + (id) * 4)
#define PLIC_PENDING(id) (PLIC_BASE + 0x1000 + ((id) / 32) * 4)
#define PLIC_MENABLE(hart, id) (PLIC_BASE + 0x2000 + (hart) * 0x100 + ((id) / 32) * 4)
#define PLIC_MTHRESHOLD(hart) (PLIC_BASE + 0x200000 + (hart) * 0x2000)
#define PLIC_MCLAIM(hart) (PLIC_BASE + 0x200004 + (hart) * 0x2000)
#define PLIC_MCOMPLETE(hart) (PLIC_BASE + 0x200004 + (hart) * 0x2000)


#define CLINT_BASE 0xffdc000000L
#define CLINT_MSIP(hartid) (CLINT_BASE + 4 * (hartid))
#define CLINT_MTIMECMP(hartid) (CLINT_BASE + 0x4000 + 8 * (hartid))
#define CLINT_MTIMECMPL(hartid) (CLINT_BASE + 0x4000 + 8 * (hartid))
#define CLINT_MTIMECMPH(hartid) (CLINT_BASE + 0x4000 + 8 * (hartid) + 4)
//#define CLINT_MTIME (CLINT_BASE + 0xBFF8) // cycles since boot.


/* 10000000 ticks per-second */
#define CLINT_TIMEBASE_FREQ 3000000
#define CLINT_SLICE_PER_SEC 60        //there will be 60 time interrupts per sec
//#define CLINT_TIMEBASE_FREQ 10000000



#define GROL_OS_VERSION "v0.05beta"


#endif /* __PLATFORM_H__ */
