#include "os.h"

/*
 * Following functions SHOULD be called ONLY ONE time here,
 * so just declared here ONCE and NOT included in file os.h.
 */


void start_kernel(void)
{
	uart_init();
	printf("Hello, GROL_OS!\n\r");//this should be replaced with graph later
	#ifdef MYPRINT
	printf("mtvec:%p\n\r", r_mtvec());
	printf("mstatus:%p\n\r", r_mstatus());
	print_printftest();
	#endif
	#ifdef MYPRINT
	printf("mtvec:%p\n\r", r_mtvec());
	printf("mstatus:%p\n\r", r_mstatus());
	//trap_test();
	#endif
	page_init();

	#ifdef MYPRINT
	page_test();
	#endif
	print_heapinit();
	swtimer_init();

	sched_init(60);

	trap_init();
	#ifdef MYPRINT
	printf("trap init done\n\r");
	#endif
	timer_init();
	#ifdef MYPRINT
	printf("timer init done\n\r");
	#endif
	plic_init();
	#ifdef MYPRINT
	printf("plic init done\n\r");
	#endif
	#ifdef MYPRINT
	printf("mtvec:%p\n\r", r_mtvec());
	printf("mstatus:%p\n\r", r_mstatus());
	printf("mie:%p\n\r", r_mie());
	#endif

	//print_multitasktest();

	os_main();
	printf("task created\n\r");

	kernel_schedule();

	printf("Would not go here!\n\r");

	while (1) {}; // stop here!
}

