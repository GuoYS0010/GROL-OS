extern void uart_init(void);
extern void uart_puts(char *s);
extern char uart_getc();
int uart_putc(char ch);

void start_kernel(void)
{
	uart_init();

	while (1) {

	char c = uart_getc();
	while (c != '\r'){
		uart_putc(c);
		c = uart_getc();
	}
	uart_putc('\r');
	uart_puts("Hello, GROL-OS!\n\r");
	}; // stop here!
}

