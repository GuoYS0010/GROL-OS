#include "user.h"

#define DELAY 1000
static struct semaphore user_sem;
void task_yield()
{
	//call_software_interrupt(SOFTWARE_USER_SWITCH_FLAG, 0);
	user_syscall(SYSCALL_JUST_YIELD);
}
void swtimer_print(){
	printf("swtimer print test*****************\n\r");
}
void user_task1(void* param)
{
	printf("\033[32mTask 1: Created!\033[0m\n\r");
	//swtimer_create(swtimer_print, 0, 20, SWTIMER_PERIODIC);
	while (1) {
		semaphore_init(&user_sem);
		semaphore_wait(&user_sem);
		for (int i=0; i < 5; i++){
			reg_t mytime;
			//get_MTIME(mytime);
			mytime = user_syscall(SYSCALL_GET_MTIME);
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
		semaphore_signal(&user_sem);
		printf("user_sem is released:%d\n\r", user_sem.value);
	}
}

void user_task2(void* param)
{
	printf("\033[32mTask 2: Created!\033[0m\n\r");
	while (1) {
		semaphore_init(&user_sem);
		semaphore_wait(&user_sem);
		for (int i=0; i < 5; i++){
			printf("\033[32mTask 2: Running...\033[0m\n\r");
			task_delay(DELAY);
			#ifndef MYPRINT
			printf("\033[1A");
			printf("\033[K");
			printf("\033[1A");
			printf("\033[K");
			#endif
		}
		printf("im ready to out\n\r");
		semaphore_signal(&user_sem);
		printf("user_sem is released:%d\n\r", user_sem.value);
	}
}
void user_task3(void* param)
{
	printf("\033[32mTask 3: Created!\033[0m\n\r");
	while (1) {
		printf("\033[32mTask 3: Running...\033[0m\n\r");
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
	//task_delay(DELAY);
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
	user_syscall(SYSCALL_KILL_MYSELF);
}

/* NOTICE: DON'T LOOP INFINITELY IN main() */
void os_main(void)
{
	task_create(user_task1, (void*)0x123456789abcdef, 2);
	task_create(user_task2, (void*)0xfedcba9876543210, 2);
	task_create(user_task3, (void*)0xdeadbeefbeefdead, 2);
	task_create(user_task4, (void*)0xdeadbeefbeefdead, 1);
}



