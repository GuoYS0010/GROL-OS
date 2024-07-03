#ifndef PRINTF_H
#define PRINTF_H
#include <stddef.h>
#include <stdarg.h>
#include "uart.h"

int  printf(const char* s, ...);
void panic(char *s);
#endif