#include "user.h"

#define DELAY 1000

void user_task1(void* param)
{
	printf("\033[32mTask 1: Created!\033[0m\n\r");
	while (1) {
		reg_t mytime;
		get_MTIME(mytime);
		printf("\033[32mTask 1: Running...\033[0m\n\r");
		printf("\033[32mCurrent time is:%d\033[0m\n\r", mytime);
		task_delay(DELAY);
		#ifndef MYPRINT
		printf("\033[1A");
		printf("\033[K");
		printf("\033[1A");
		printf("\033[K");
		#endif
		task_yield();
	}
}

void user_task2(void* param)
{
	printf("\033[32mTask 2: Created!\033[0m\n\r");
	while (1) {
		printf("\033[32mTask 2: Running...\033[0m\n\r");
		printf("\033[32mMy cmp is:%p\033[0m\n\r", *(uint64_t*)CLINT_MTIMECMP(0));
		task_delay(DELAY);
		#ifndef MYPRINT
		printf("\033[1A");
		printf("\033[K");
		printf("\033[1A");
		printf("\033[K");
		#endif
		task_yield();
	}
}

void user_task3(void* param)
{
	printf("\033[32mTask 3: Created!\033[0m\n\r");
	printf("\033[32mTask 3: Running...\033[0m\n\r");
	printf("\033[32mMy output is:%p\033[0m\n\r", param);
	task_delay(DELAY);
	#ifndef MYPRINT
	printf("\033[1A");
	printf("\033[K");
	printf("\033[1A");
	printf("\033[K");
	#endif
	print_taskkilled();
	task_delay(DELAY);
	#ifndef MYPRINT
	delete_taskkilled();
	#endif
	printf("\033[31mTask 3: Killed!\033[0m\n\r");
	task_exit();
}

/* NOTICE: DON'T LOOP INFINITELY IN main() */
void os_main(void)
{
	task_create(user_task1, (void*)0x123456789abcdef, 1);
	task_create(user_task2, (void*)0xfedcba9876543210, 2);
	task_create(user_task3, (void*)0xdeadbeefbeefdead, 3);
}

