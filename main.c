#include "stdio.h"

void uart_init(void);

int main(void)
{
    uart_init();
    key_init();	

    printf("test uart ok!\r\n");

    while(1)
    {
	key_polling();
    }
	/*
	while(1)
	{
		uart_putc('a');
		delay();
	}
	*/
	return 0;
}
