#include "os.h"

/*
 * Following functions SHOULD be called ONLY ONE time here,
 * so just declared here ONCE and NOT included in file os.h.
 */
extern void uart_init(void);
extern void page_init(void);

void start_kernel(void)
{
	uart_init();
	uart_puts("Hello, GROL_OS!\n\r");//this should be replaced with graph later
	#ifdef MYPRINT
	printf("#########################################################\n\r");
	printf("##                    GROL_OS                          ##\n\r");
	printf("##     this is a printf test:%p        ##\n\r",0xfedcba9876543210);
	printf("##         fedcba9876543210 means right output         ##\n\r");
	printf("#########################################################\n\r");
	#endif
	page_init();
	#ifdef MYPRINT
	page_test();
	#endif

	while (1) {}; // stop here!
}

