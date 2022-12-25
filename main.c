
int main(void)
{
	uart_init();
	
	
	while(1)
	{
	    uart_putc('a');
	    uart_putc('b');
	    uart_putc('c');
            delay();
	}
	return 0;
}
