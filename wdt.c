#include "stdio.h"
#include "init.h"

#define		WTCON		(0xE2700000)
#define		WTDAT		(0xE2700004)
#define		WTCNT		(0xE2700008)
#define 	WTCLRINT	(0xE270000C)

#define 	rWTCON		(*(volatile unsigned int *)WTCON)
#define 	rWTDAT		(*(volatile unsigned int *)WTDAT)
#define 	rWTCNT		(*(volatile unsigned int *)WTCNT)
#define 	rWTCLRINT	(*(volatile unsigned int *)WTCLRINT)


unsigned int pow(unsigned int base, unsigned int exponent)
{
    if (base == 2)
    {
        return (0x1 << exponent);
    }
    else
    {
	unsigned int sum = 1, temp = 0;
	for (temp = 0; temp < exponent; temp++)
	{
	    sum *= base;
	}

	return sum;
    }
}

/*************************************************************************/

#define         BIT_LOCATION_WTCON_PRESCALER_VALUE     (0xff << 8)
#define         BIT_WTCON_PRESCALER                    (pow(2,8))


#define         WTCON_FUNC_WDT_TIMER_ENABLE            (0x1  << 5)
#define         WTCON_FUNC_WDT_TIMER_DISABLE           (0x0  << 5)

#define         BIT_LOCATION_WTCON_CLOCK_SELECT        (0b11 << 3)
#define         WTCON_FUNC_CLOCK_SELECT_16             (0b00 << 3)
#define         WTCON_FUNC_CLOCK_SELECT_32             (0b01 << 3)
#define         WTCON_FUNC_CLOCK_SELECT_64             (0b10 << 3)
#define         WTCON_FUNC_CLOCK_SELECT_128            (0b11 << 3)

#define         WTCON_FUNC_INTERRUPT_GENERATION_ENABLE (0x1  << 2)
#define         WTCON_FUNC_INTERRUPT_GENERATION_DISABLE (0x0 << 2)

#define         BIT_LOCATION_WTCON_RESET               (0b1  << 0)
#define         WTCON_FUNC_RESET_ENABLE                (0x1  << 0)
#define         WTCON_FUNC_RESET_DISABLE               (0x0  << 0)


//初始化 WDT 使之可以产生中断
void wdt_init_interrupt(void)
{
    //第一步，设置好预分频器和分频器，得到时钟周期是 128 us
    rWTCON &= ~(BIT_LOCATION_WTCON_PRESCALER_VALUE);
    //t_watchdog = 1/( PCLK / (Prescaler value + 1) / Division_factor )
    rWTCON |= (65 * BIT_WTCON_PRESCALER); // 1MHZ

    rWTCON &= ~(BIT_LOCATION_WTCON_CLOCK_SELECT);
    rWTCON |= (WTCON_FUNC_CLOCK_SELECT_128);  // 1/128 MHZ, T = 128us

    //第二步，设置中断和复位信号的使能或禁止
    rWTCON |= (WTCON_FUNC_INTERRUPT_GENERATION_ENABLE); // enable wdt interrupt

    rWTCON &= ~(BIT_LOCATION_WTCON_RESET);  
    rWTCON |= (WTCON_FUNC_RESET_DISABLE);   //disable wdt reset

    //第三步，设置定时时间
    //WDT 定时计数个数，最终定时时间为这里的值 x 时钟周期
    rWTDAT = 10000;   //定时 1.28s
    rWTCNT = 10000;   //定时 1.28s

    //第四步，先把所有寄存器都设置好之后，再去打开看门狗
    rWTCON |= (WTCON_FUNC_WDT_TIMER_ENABLE);  //enable wdt
}

//wdt 的中断处理程序
void isr_wdt(void)
{
    static int i = 0;
    //看门狗定时器时间到了，应该做的有意义的事情
    printf("wdt interrupt, i = %d...\r\n", i++);

    //清除中断
    intc_clearVectaddr();
    rWTCLRINT = 1;
}
