#include "os.h"
#include "graphprint/graphprint.h"

/*
 * Following functions SHOULD be called ONLY ONE time here,
 * so just declared here ONCE and NOT included in file os.h.
 */
extern void uart_init(void);
extern void page_init(void);
extern void sched_init(void);
extern void schedule(void);
extern void os_main(void);

extern reg_t r_mtvec();

void start_kernel(void)
{
	uart_init();
	uart_puts("Hello, GROL_OS!\n\r");//this should be replaced with graph later
	#ifdef MYPRINT
	printf("mtvec:%p\n\r", r_mtvec());
	printf("mstatus:%p\n\r", r_mstatus());
	print_printftest();
	#endif
	page_init();
	print_heapinit();
	#ifdef MYPRINT
	page_test();
	#endif
	print_multitasktest();
	sched_init();

	os_main();

	schedule();

	uart_puts("Would not go here!\n\r");

	while (1) {}; // stop here!
}

