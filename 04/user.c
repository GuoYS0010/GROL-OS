#include "os.h"
#include "graphprint/graphprint.h"

#define DELAY 1000

extern void task_exit();
extern uint8_t get_priority();
void user_task1(void* param)
{
	uart_puts("Task 1: Created!\n\r");
	while (1) {
		uart_puts("Task 1: Running...\n\r");
		printf("My output is:%p\n\r", param);
		printf("My priority is:%d\n\r", get_priority());
		task_delay(DELAY);
		task_yield();
	}
}

void user_task2(void* param)
{
	uart_puts("Task 2: Created!\n\r");
	while (1) {
		uart_puts("Task 2: Running...\n\r");
		printf("My output is:%p\n\r", param);
		printf("My priority is:%d\n\r", get_priority());
		task_delay(DELAY);
		task_yield();
	}
}

void user_task3(void* param)
{
	uart_puts("Task 3: Created!\n\r");
	uart_puts("Task 3: Running...\n\r");
	printf("My output is:%p\n\r", param);
	printf("My priority is:%d\n\r", get_priority());
	task_delay(DELAY);
	print_taskkilled();
	task_delay(DELAY);
	task_exit();
}

/* NOTICE: DON'T LOOP INFINITELY IN main() */
void os_main(void)
{
	task_create(user_task1, (void*)0x123456789abcdef, 1);
	task_create(user_task2, (void*)0xfedcba9876543210, 2);
	task_create(user_task3, (void*)0xdeadbeefbeefdead, 3);
}

