#include "stdio.h"
#include "init.h"
#include "main.h"
#include "wdt.h"

void uart_init(void);

#define  KEY_EINT2        NUM_EINT2      //left
#define  KEY_EINT3        NUM_EINT3      //down
#define  KEY_EINT16_19    NUM_EINT16_31  // 其余 4 个共用的

#define  KEY_INTERRUPT    0
#define  PWM_TIMER2       0

int main(void)
{
    uart_init();
    wdt_init_interrupt();


    //如果程序要使用中断,就要调用中断初始化来初步初始化中断控制器
    system_init_exception();

    printf("---------- wdt interrupt -----------\r\n");

    //绑定 isr 到中断控制器硬件
    intc_setVectaddr(NUM_WDT, isr_wdt);

    //使能中断
    intc_enable(NUM_WDT);


    while(1)
    {
	printf("A, pow(10,5)=%d\r\n", pow(10,5));
	delay_seconds(1);
    }


#if PWM_TIMER2
    uart_init();

    timer2_pwm_init();

    while(1)
    {
	printf("A\r\n");
	delay_seconds(1);
    }
#endif

#if KEY_INTERRUPT	
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
#endif
 
	/*
	while(1)
	{
		uart_putc('a');
		delay();
	}
	*/
	return 0;
}
