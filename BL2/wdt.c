#include "stdio.h"
#include "init.h"
#include "shell.h"

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

#define         BIT_LOCATION_WTCON_INTERRUPT           (0b1  << 2)
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
    rWTCON &= ~(BIT_LOCATION_WTCON_INTERRUPT); 
    //rWTCON |= (WTCON_FUNC_INTERRUPT_GENERATION_DISABLE); // disable wdt interrupt
    rWTCON |= (WTCON_FUNC_INTERRUPT_GENERATION_ENABLE); // disable wdt interrupt

    //rWTCON &= ~(BIT_LOCATION_WTCON_RESET);  
    //rWTCON |= (WTCON_FUNC_RESET_ENABLE);   //enable wdt reset

    //第三步，设置定时时间
    //WDT 定时计数个数，最终定时时间为这里的值 x 时钟周期
    //rWTDAT = 10000;   //定时 1.28s
    //rWTCNT = 10000;   //定时 1.28s
    rWTDAT = 7812;		// 定时1s
	rWTCNT = 7812;		// 定时1s

    //第四步，先把所有寄存器都设置好之后，再去打开看门狗
    rWTCON |= (WTCON_FUNC_WDT_TIMER_ENABLE);  //enable wdt
}

//wdt 的中断处理程序
void isr_wdt(void)
{
    static int i = 0;
    // 看门狗定时器时间到了时候应该做的有意义的事情
	//printf("wdt interrupt, i = %d...", i++);
	// 计时，然后时间没到的时候在屏幕上打印倒数计数，时间到了自动执行命令
	// 执行完命令进入shell的死循环
	g_bootdelay--;
	putchar('\b');
	printf("%d", g_bootdelay);
	
	if (g_bootdelay == 0)
	{
		g_isgo = 1;
		
		// 把要自动执行的命令添加到这里，但是这里是中断处理程序，不适合执行长代码
		// 所以放在外面要好一些
		
		printf("%s g_isgo = 1.\r\n", __func__);
		// 关闭wdt
		intc_disable(NUM_WDT);
	}

	// 清中断
	intc_clearVectaddr();
	rWTCLRINT = 1;
}

void wdt_timer_init()
{
    wdt_init_interrupt();

    //如果程序要使用中断,就要调用中断初始化来初步初始化中断控制器
    system_init_exception();

    //printf("---------- wdt interrupt -----------\r\n");

    //绑定 isr 到中断控制器硬件
    intc_setVectaddr(NUM_WDT, isr_wdt);

    //使能中断
    intc_enable(NUM_WDT);

    //while(1)
    //{
	//printf("A, pow(10,5)=%d\r\n", pow(10,5));
	//delay_seconds(1);
    //}
}