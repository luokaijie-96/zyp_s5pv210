#define 	GPD0CON		(0xE02000A0)
#define 	TCFG0		(0xE2500000)
#define 	TCFG1		(0xE2500004)
#define 	CON		    (0xE2500008)
#define 	TCNTB2		(0xE2500024)
#define 	TCMPB2		(0xE2500028)

#define 	rGPD0CON	(*(volatile unsigned int *)GPD0CON)
#define 	rTCFG0		(*(volatile unsigned int *)TCFG0)
#define 	rTCFG1		(*(volatile unsigned int *)TCFG1)
#define 	rCON		(*(volatile unsigned int *)CON)
#define 	rTCNTB2		(*(volatile unsigned int *)TCNTB2)
#define 	rTCMPB2		(*(volatile unsigned int *)TCMPB2)

/******************************************************************************/

#define         BIT_WIDTH_GPD0_CON                (4)
#define         BIT_LOCATION_GPD0_2_FUNC          (0xf    << (2 * BIT_WIDTH_GPD0_CON))
#define         GPD0_2_FUNC_INPUT                 (0b0000 << (2 * BIT_WIDTH_GPD0_CON))
#define         GPD0_2_FUNC_OUTPUT                (0b0001 << (2 * BIT_WIDTH_GPD0_CON))
#define         GPD0_2_FUNC_TOUT2                 (0b0010 << (2 * BIT_WIDTH_GPD0_CON))
#define         GPD0_2_FUNC_INT2                  (0b1111 << (2 * BIT_WIDTH_GPD0_CON))

/******************************************************************************/

#define         BIT_LOCATION_TCFG0_PRESCALER0     (0xff)   // 1~255
#define         BIT_LOCATION_TCFG0_PRESCALER1     (0xff << 8) // 1~255

/******************************************************************************/

#define         BIT_WIDTH_TCFG1_DIVIDER            (4)
#define         BIT_LOCATION_TCFG1_DIVIDER_MUX2    (0xf    << (2 * BIT_WIDTH_TCFG1_DIVIDER))
#define         TCFG1_DIVIDER_MUX2_DIVID_1         (0b0000 << (2 * BIT_WIDTH_TCFG1_DIVIDER))
#define         TCFG1_DIVIDER_MUX2_DIVID_2         (0b0001 << (2 * BIT_WIDTH_TCFG1_DIVIDER))
#define         TCFG1_DIVIDER_MUX2_DIVID_4         (0b0010 << (2 * BIT_WIDTH_TCFG1_DIVIDER))
#define         TCFG1_DIVIDER_MUX2_DIVID_8         (0b0011 << (2 * BIT_WIDTH_TCFG1_DIVIDER))
#define         TCFG1_DIVIDER_MUX2_DIVID_16        (0b0100 << (2 * BIT_WIDTH_TCFG1_DIVIDER))
#define         TCFG1_DIVIDER_MUX2_DIVID_SCLK_PWM  (0b0101 << (2 * BIT_WIDTH_TCFG1_DIVIDER))

/******************************************************************************/

#define         BIT_LOCATION_TCON_TIMER2_START_STOP         (0b1 << 12)
#define         TCON_TIMER2_FUNC_STOP                       (0b0 << 12)
#define         TCON_TIMER2_FUNC_START                      (0b1 << 12)

#define         BIT_LOCATION_TCON_TIMER2_MANUAL_UPDATE      (0b1 << 13)
#define         TCON_TIMER2_FUNC_NO_UPDATE                  (0b0 << 13)
#define         TCON_TIMER2_FUNC_MANUAL_UPDATE              (0b1 << 13)

#define         BIT_LOCATION_TCON_TIMER2_OUTPUT_INVERTER    (0b1 << 14)
#define         TCON_TIMER2_FUNC_TOUT2_INVERTER_OFF         (0b0 << 14)
#define         TCON_TIMER2_FUNC_TOUT2_INVERTER_ON          (0b1 << 14)

#define         BIT_LOCATION_TCON_TIMER2_AUTO_RELOAD        (0b1 << 15)
#define         TCON_TIMER2_FUNC_AUTO_RELOAD_OFF            (0b0 << 15)
#define         TCON_TIMER2_FUNC_AUTO_RELOAD_ON             (0b1 << 15)

enum {
    PWM_TRISTATE = 0,
    PWM_OFF,
    PWM_ON,
    PWM_MAX
} pwm_state_e;

static unsigned int is_pwm_on = PWM_TRISTATE;

//初始化 PWM Timer2，使其输出 PWM 波形: 频率是 2KHZ, duty 为 50%
void timer2_pwm_init()
{
    //设置 GPD0_2 引脚，将其配置为 XpwmTOUT_2
    rGPD0CON &= ~(BIT_LOCATION_GPD0_2_FUNC);
    rGPD0CON |= (GPD0_2_FUNC_TOUT2);


    //Timer Input Clock Frequency = PCLK / ( {prescaler value + 1} ) / {divider value}
    //设置 PWM 定时器的若干寄存器, 使其工作
    rTCFG0 &= ~(BIT_LOCATION_TCFG0_PRESCALER1);
    rTCFG0 |= (65 << 8);  // 66 分频

    rTCFG1 &= ~(BIT_LOCATION_TCFG1_DIVIDER_MUX2);
    rTCFG1 |= TCFG1_DIVIDER_MUX2_DIVID_2; //MUX2 设置为 1/2，分频后时钟周期为 500KHZ
    //时钟设置好，我们的时钟频率是 500KHZ, 对应的时钟周期是 2us.也就是说每隔 2us
    //计一次数. 如果要定的时间是 x，则 TCNTB 中应该写入 x/2us
    
    
    rCON |= (TCON_TIMER2_FUNC_AUTO_RELOAD_ON);  //使能 auto-reload,反复定时才能发出 PWM 波形
    
    //设置 PWM 波形为 2KHZ, 即周期是 0.5ms; 500us/2us = 250,即计数 250,就可以定时 0.5ms.
    rTCNTB2 = 250;
    rTCMPB2 = 125;

    //设置 PWM 波形为 10KHZ, 即周期是 0.1ms; 100us/2us = 50,即计数 50,就可以定时 0.1ms.
    rTCNTB2 = 50;
    rTCMPB2 = 25;

    //设置 PWM 波形为 100HZ, 即周期是 10ms; 10,000us/2us = 5000, 即计数 5000,就可以定时 10ms.
    rTCNTB2 = 5000;
    rTCMPB2 = 2500;

    //第一次需要手工将 TCNTB 中的值刷新到 TCNT 中, 以后就可以 auto-reload 了
    rCON |= TCON_TIMER2_FUNC_MANUAL_UPDATE;            //打开自动刷新功能
    rCON &= ~(BIT_LOCATION_TCON_TIMER2_MANUAL_UPDATE); //关闭自动刷新功能

    //rCON |= (TCON_TIMER2_FUNC_START);   //开 timer2 定时器,要先把其他设置好才能开定时器

    is_pwm_on = PWM_TRISTATE;
    buzzer_off();
}


void buzzer_on(void)
{
    if (is_pwm_on == PWM_ON)
    {
        return;
    }

    rGPD0CON &= ~(BIT_LOCATION_GPD0_2_FUNC);
    rGPD0CON |= (GPD0_2_FUNC_TOUT2);

    rCON &= ~(BIT_LOCATION_TCON_TIMER2_START_STOP);
    rCON |= (TCON_TIMER2_FUNC_START); 

    is_pwm_on = PWM_ON;
}

// 关蜂鸣器的思路：TCON中设置关，GPIO设置成其他模式
void buzzer_off(void)
{
    if (is_pwm_on == PWM_OFF)
    {
        return;
    }

    rGPD0CON &= ~(BIT_LOCATION_GPD0_2_FUNC);
    rGPD0CON |= (GPD0_2_FUNC_INPUT);

    rCON &= ~(BIT_LOCATION_TCON_TIMER2_START_STOP);
    rCON |= (TCON_TIMER2_FUNC_STOP); 

    is_pwm_on = PWM_OFF;
}


void buzzer_set_freq(unsigned int freq)
{
    //初始时钟频率是 500KHZ
    unsigned int state = is_pwm_on;

    if (state == PWM_ON)
    {
        buzzer_off();
    }

    switch (freq)
    {
        case 2000:
            rTCNTB2 = 250;
            rTCMPB2 = 125;
            break;
        case 10000:
            rTCNTB2 = 50;
            rTCMPB2 = 25;
            break;
        case 100:
            rTCNTB2 = 5000;
            rTCMPB2 = 2500;
            break;        
        default:
            break;
    }

    if (state == PWM_ON)
    {
        buzzer_on();
    }

    #if 0
    //设置 PWM 波形为 2KHZ, 即周期是 0.5ms; 500us/2us = 250,即计数 250,就可以定时 0.5ms.
    rTCNTB2 = 250;
    rTCMPB2 = 125;

    //设置 PWM 波形为 10KHZ, 即周期是 0.1ms; 100us/2us = 50,即计数 50,就可以定时 0.1ms.
    rTCNTB2 = 50;
    rTCMPB2 = 25;

    //设置 PWM 波形为 100HZ, 即周期是 10ms; 10,000us/2us = 5000, 即计数 5000,就可以定时 10ms.
    rTCNTB2 = 5000;
    rTCMPB2 = 2500;
    #endif
}

// 现象：引脚直接配置成输出模式，然后输出高电平，buzzer叫了。输出低电平就关了
// 优点就是SOC不用有pwm功能，只要能输出高低电平就能控制蜂鸣器
// 缺点是蜂鸣器的鸣叫的频率无法改动
// 如果可以，那么开关蜂鸣器就变成了GPIO输出高低电平了。


