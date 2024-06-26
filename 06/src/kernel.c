#include "os.h"

/*
 * Following functions SHOULD be called ONLY ONE time here,
 * so just declared here ONCE and NOT included in file os.h.
 */


void start_kernel(void)
{
	uart_init();
	uart_puts("Hello, GROL_OS!\n\r");//this should be replaced with graph later
	#ifdef MYPRINT
	printf("mtvec:%p\n\r", r_mtvec());
	printf("mstatus:%p\n\r", r_mstatus());
	print_printftest();
	#endif
	trap_init();
	#ifdef MYPRINT
	printf("mtvec:%p\n\r", r_mtvec());
	printf("mstatus:%p\n\r", r_mstatus());
	//trap_test();
	#endif
	sched_init();
	plic_init();
	timer_init();
	#ifdef MYPRINT
	printf("mtvec:%p\n\r", r_mtvec());
	printf("mstatus:%p\n\r", r_mstatus());
	printf("mie:%p\n\r", r_mie());
	#endif

	page_init();
	print_heapinit();
	#ifdef MYPRINT
	page_test();
	#endif
	print_multitasktest();

	os_main();

	schedule();

	uart_puts("Would not go here!\n\r");

	while (1) {}; // stop here!
}

