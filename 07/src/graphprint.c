#include "graphprint.h"

void print_printftest(){
	printf("#########################################################\n\r");
	printf("##                    GROL_OS                          ##\n\r");
	printf("##     this is a printf test:%p        ##\n\r",0xfedcba9876543210);
	printf("##         fedcba9876543210 means right output         ##\n\r");
	printf("#########################################################\n\r");
}

void print_WhoAmI(){
	printf("\n\r");
	printf("\033[5m#########################################################\n\r");
	printf("##                    GROL_OS                          ##\n\r");
	printf("##                   %s                         ##\n\r", GROL_OS_VERSION);
	printf("#########################################################\033[0m\r");
	#ifndef MYPRINT
	printf("\033[1A");
	printf("\033[1A");
	printf("\033[1A");
	printf("\033[1A");
	#endif
}

void print_heapinit(){
	printf("#########################################################\n\r");
	printf("##                    GROL_OS                          ##\n\r");
	printf("##                HEAP INIT SUCCESS...                 ##\n\r");
	printf("#########################################################\n\r");
}

void print_multitasktest(){
	printf("#########################################################\n\r");
	printf("##                    GROL_OS                          ##\n\r");
	printf("##              2  cooperative task test...            ##\n\r");
	printf("#########################################################\n\r");
}
void print_taskkilled(){
	printf("\033[31m#########################################################\033[0m\n\r");
	printf("\033[31m##                    GROL_OS                          ##\033[0m\n\r");
	printf("\033[31m##              AHAHAH  I AM KILLED.........           ##\033[0m\n\r");
	printf("\033[31m#########################################################\033[0m\n\r");
}
void delete_taskkilled(){
	printf("\033[1A");
	printf("\033[K");
	printf("\033[1A");
	printf("\033[K");
	printf("\033[1A");
	printf("\033[K");
	printf("\033[1A");
	printf("\033[K");
}
void print_taskcontroller_created(){
	printf("#########################################################\n\r");
	printf("##                    GROL_OS                          ##\n\r");
	printf("##              TASK CONTROLLER CREATED                ##\n\r");
	printf("#########################################################\n\r");
}