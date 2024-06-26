#include "user.h"

#define DELAY 1000
struct mutex mymutex = {MUTEX_IS_FREE_FLAG};

void user_task1(void* param)
{
	printf("\033[32mTask 1: Created!\033[0m\n\r");
	while (1) {
		mutex_acquire(&mymutex);
		printf("mutex is get:%d\n\r", mymutex.flag);
		for (int i=0; i < 5; i++){
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
		}
		printf("im ready to out\n\r");
		mutex_release(&mymutex);
		printf("mutex is released:%d\n\r", mymutex.flag);
	}
}

void user_task2(void* param)
{
	printf("\033[32mTask 2: Created!\033[0m\n\r");
	while (1) {
		mutex_acquire(&mymutex);
		printf("mutex is get:%d\n\r", mymutex.flag);
		for (int i=0; i < 5; i++){
			printf("\033[32mTask 2: Running...\033[0m\n\r");
			printf("\033[32mMy cmp is:%p\033[0m\n\r", *(uint64_t*)CLINT_MTIMECMP(0));
			task_delay(DELAY);
			#ifndef MYPRINT
			printf("\033[1A");
			printf("\033[K");
			printf("\033[1A");
			printf("\033[K");
			#endif
		}
		printf("im ready to out\n\r");
		mutex_release(&mymutex);
		printf("mutex is released:%d\n\r", mymutex.flag);
	}
}
void user_task3(void* param)
{
	printf("\033[32mTask 3: Created!\033[0m\n\r");
	while (1) {
		printf("\033[32mTask 3: Running...\033[0m\n\r");
		printf("\033[32mCurrent time is:%p\033[0m\n\r", r_time());
		printf("\033[32mMy cmp is:%p\033[0m\n\r", r_mtimecmp());
		task_delay(DELAY);
		#ifndef MYPRINT
		printf("\033[1A");
		printf("\033[K");
		printf("\033[1A");
		printf("\033[K");
		#endif
	}
}

void user_task4(void* param)
{
	printf("\033[32mTask 4: Created!\033[0m\n\r");
	printf("\033[32mTask 4: Running...\033[0m\n\r");
	printf("\033[32mMy output is:%p\033[0m\n\r", param);
	task_delay(DELAY);
	#ifndef MYPRINT
	printf("\033[1A");
	printf("\033[K");
	printf("\033[1A");
	printf("\033[K");
	#endif
	#ifndef MYPRINT
	print_taskkilled();
	delete_taskkilled();
	#endif
	printf("\033[31mTask 4: Killed!\033[0m\n\r");
	task_exit();
}

/* NOTICE: DON'T LOOP INFINITELY IN main() */
void os_main(void)
{
	task_create(user_task1, (void*)0x123456789abcdef, 2);
	task_create(user_task2, (void*)0xfedcba9876543210, 2);
	task_create(user_task3, (void*)0xdeadbeefbeefdead, 2);
	task_create(user_task4, (void*)0xdeadbeefbeefdead, 1);
}



