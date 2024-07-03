#include "semaphore.h"

void fifo_pop(uint64_t *list){
    for (int i=0; i < 9; i++) list[i] = list[i+1];
    list[9] = 0;
}