#include "stdio.h"
#include "init.h"
#include "main.h"

void uart_init(void);

#define  KEY_EINT2        NUM_EINT2      //left
#define  KEY_EINT3        NUM_EINT3      //down
#define  KEY_EINT16_19    NUM_EINT16_31  // 其余 4 个共用的



int main(void)
{
    uart_init();
    //key_init();	
    //key 外部中断寄存器初始化
    key_init_interrupt();

    //如果程序要使用中断,就要调用中断初始化来初步初始化中断控制器
    system_init_exception();

    printf("---------- key interrupt -----------\r\n");

    //绑定 isr 到中断控制器硬件
    intc_setVectaddr(KEY_EINT2, isr_eint2);
    intc_setVectaddr(KEY_EINT3, isr_eint3);
    intc_setVectaddr(KEY_EINT16_19, isr_eint16171819);

    //使能中断
    intc_enable(KEY_EINT2);
    intc_enable(KEY_EINT3);
    intc_enable(KEY_EINT16_19);

    printf("init: VIC0INTENABLE [%p]\r\n.", VIC0INTENABLE);
    int cnt = 0;


    //在这里加个心跳
    while(1)
    {
        cnt++;
        printf("A\r\n");
        if (cnt == 10)
        {
            //VIC0INTENCLEAR |= 0xc;
            intc_disable(KEY_EINT2);
            intc_disable(KEY_EINT3);
            printf("clear: VIC0INTENABLE [%p].\r\n", VIC0INTENABLE);
        }
        delay_seconds(1);
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
