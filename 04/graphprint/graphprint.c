#include "graphprint.h"

void print_printftest(){
	printf("#########################################################\n\r");
	printf("##                    GROL_OS                          ##\n\r");
	printf("##     this is a printf test:%p        ##\n\r",0xfedcba9876543210);
	printf("##         fedcba9876543210 means right output         ##\n\r");
	printf("#########################################################\n\r");
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
	printf("#########################################################\n\r");
	printf("##                    GROL_OS                          ##\n\r");
	printf("##              AHAHAH  I AM KILLED.........           ##\n\r");
	printf("#########################################################\n\r");
}
void print_taskcontroller_created(){
	printf("#########################################################\n\r");
	printf("##                    GROL_OS                          ##\n\r");
	printf("##              TASK CONTROLLER CREATED                ##\n\r");
	printf("#########################################################\n\r");
}