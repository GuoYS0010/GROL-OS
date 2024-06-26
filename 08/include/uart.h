#ifndef UART_H
#define UART_H

#include "types.h"
#include "platform.h"
#include "goish.h"
#include "printf.h"

/* uart */
int uart_putc(char ch);
void uart_puts(char *s);
void uart_init();
int uart_getc(void);
void uart_isr(void);
uint32_t get_uart_usr(void);
#endif