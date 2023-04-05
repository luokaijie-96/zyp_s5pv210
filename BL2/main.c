#include "stdio.h"
#include "init.h"
#include "main.h"
#include "wdt.h"
#include "rtc.h"
#include "shell.h"

void uart_init(void);

#define  KEY_EINT2        NUM_EINT2      //left
#define  KEY_EINT3        NUM_EINT3      //down
#define  KEY_EINT16_19    NUM_EINT16_31  // 其余 4 个共用的

#define  KEY_INTERRUPT    0
#define  PWM_TIMER2       0
#define  WDT_FUNC         0
#define  RTC_FUNC         0
#define  ADC_FUNC         0


#define  PROMPT      "X210 SHELL# "


int g_isgo = 0;				// 如果等于0则不能继续执行，等于1则可以继续执行了
int g_bootdelay = 3;		// 等待时间

static void hardware_init(void)
{
	led_init();				// led初始化
	lcd_init();				// lcd初始化
	timer2_pwm_init();		// 蜂鸣器初始化
	adc_init();				// adc初始化
}



// 全局变量初始化
void global_init(void)
{
	char val[MAX_LEN_PART] = {0};
	if (env_get("bootdelay", val))
	{
		printf("env bootdelay not found.\r\n");
		return;
	}
    //printf("%s --- g_bootdelay %d\r\n", __func__, g_bootdelay);
	// 字符串转数字，字符串格式的"5"转成数字格式的5赋值给g_bootdelay
    g_bootdelay = simple_strtoul(val, (void*)0, 10);
    //printf("%s --- g_bootdelay %d\r\n", __func__, g_bootdelay);
}

static void shell_init(void)
{
	// shell init
	init_cmd_set();
	uart_init();
    puts("^o^ X210 simple shell ^o^\n");		// shell logo

    hardware_init();
    g_isgo = 0;
    g_bootdelay = 3;

    // 环境变量初始化
	env_init();
    global_init();


    //开机倒计时
    wdt_timer_init();					// 看门狗
}

static void shell_loop(void)
{
	char buf[MAX_LINE_LENGTH] = {0};		// 用来暂存用户输入的命令
	
	while(1)
	{
		// 第1步：命令获取
    	puts(PROMPT);
    	memset(buf, 0, sizeof(buf));		// buf弄干净好存储这次用户输入
    	gets(buf);							// 读取用户输入放入buf中
    	// 第2步：命令解析
    	cmd_parser(buf);
    	// 第3步：命令执行
    	cmd_exec();
	}
}

static void delay(void)
{
	volatile unsigned int i, j;
	for (i=0; i<2000; i++)
		for (j=0; j<1000; j++);
}

int main(void)
{
    shell_init();

	// 自动倒数执行默认命令
	// 在这里等待用户按按键，如果没按就倒计时，如果按了就结束倒计时和自动执行直接
	// 进入shell死循环。如果一直没按按键时间到了也进入shell死循环
	puts(PROMPT);
	printf("%d", g_bootdelay);
	while (!((g_isgo) || (is_keyboard_press())));
	
	intc_disable(NUM_WDT);

    printf("\r\n");
    print_help();
	// 也可以在这里通过判断g_isgo的值来判断是不是倒数结束，执行自动命令
	//if (g_isgo)
	//{
	//	lcd_test();
	//}

    
	// 执行shell死循环
	shell_loop();
    
    return 0;

#if ADC_FUNC
    uart_init();
    printf("-------- adc test ---------\r\n");
    adc_test();


    while (1) ;

    return 0;
#endif


#if RTC_FUNC	
    uart_init();
    printf("-------- rtc write time test ---------\r\n");
    struct rtc_time tWrite = 
    {   
        .year = 2023,
	.month = 1,
	.date = 16,
	.hour = 14,
	.minute = 22,
	.second = 8,
	.day = 1,
    };

    rtc_set_time(&tWrite);


    //printf("-------- rtc read time test ---------\r\n");
    //struct rtc_time  tRead;
    //while (1)
    //{
    //    rtc_get_time(&tRead);
    //    printf("The time read is: %d-%d-%d %d:%d:%d %d\r\n", tRead.year, tRead.month,
    //    		    tRead.date, tRead.hour, tRead.minute, tRead.second, tRead.day);

    //    delay_seconds(1);
    //}
    
    system_init_exception();
    rtc_set_alarm();

    intc_setVectaddr(NUM_RTC_ALARM, isr_rtc_alarm);
    intc_enable(NUM_RTC_ALARM);

    struct rtc_time  tRead;
    while (1)
    {
        rtc_get_time(&tRead);
        printf("The time read is: %d-%d-%d %d:%d:%d %d\r\n", tRead.year, tRead.month,
        		    tRead.date, tRead.hour, tRead.minute, tRead.second, tRead.day);

        delay_seconds(1);
    }
#endif    


#if WDT_FUNC
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
#endif

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
