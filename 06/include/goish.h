#ifndef __GOISH_H__
#define __GOISH_H__
#include "platform.h"
#include "types.h"
#include "uart.h"
#define GOISH_BACKSPACE     8
#define GOISH_TAB           9
#define GOISH_ENTER         13
void addGoishBuffer(char c);
#endif