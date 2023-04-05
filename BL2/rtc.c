#include "rtc.h"
#include "init.h"
#include "stdio.h"

#define 	RTC_BASE	 (0xE2800000)
#define		rINTP      	 (*((volatile unsigned long *)(RTC_BASE + 0x30)))
#define		rRTCCON    	 (*((volatile unsigned long *)(RTC_BASE + 0x40)))
#define		rTICCNT    	 (*((volatile unsigned long *)(RTC_BASE + 0x44)))
#define		rRTCALM    	 (*((volatile unsigned long *)(RTC_BASE + 0x50)))
#define		rALMSEC    	 (*((volatile unsigned long *)(RTC_BASE + 0x54)))
#define		rALMMIN    	 (*((volatile unsigned long *)(RTC_BASE + 0x58)))
#define		rALMHOUR  	 (*((volatile unsigned long *)(RTC_BASE + 0x5c)))
#define		rALMDATE         (*((volatile unsigned long *)(RTC_BASE + 0x60)))
#define		rALMMON    	 (*((volatile unsigned long *)(RTC_BASE + 0x64)))
#define		rALMYEAR  	 (*((volatile unsigned long *)(RTC_BASE + 0x68)))
#define		rRTCRST          (*((volatile unsigned long *)(RTC_BASE + 0x6c)))
#define		rBCDSEC    	 (*((volatile unsigned long *)(RTC_BASE + 0x70)))
#define		rBCDMIN   	 (*((volatile unsigned long *)(RTC_BASE + 0x74)))
#define		rBCDHOUR         (*((volatile unsigned long *)(RTC_BASE + 0x78)))
#define		rBCDDATE         (*((volatile unsigned long *)(RTC_BASE + 0x7c)))
#define		rBCDDAY          (*((volatile unsigned long *)(RTC_BASE + 0x80)))
#define		rBCDMON          (*((volatile unsigned long *)(RTC_BASE + 0x84)))
#define		rBCDYEAR         (*((volatile unsigned long *)(RTC_BASE + 0x88)))
#define		rCURTICCNT       (*((volatile unsigned long *)(RTC_BASE + 0x90)))
#define		rRTCLVD    	 (*((volatile unsigned long *)(RTC_BASE + 0x94)))

/**********************************************************************************/
#define         BIT_LOCATION_RTCCON_RTCEN_CONTROL       (0b1 << 0)
#define         RTCCON_FUNC_RTC_CONTROL_ENABLE          (0b1 << 0)
#define         RTCCON_FUNC_RTC_CONTROL_DISABLE         (0b0 << 0)

/**********************************************************************************/
#define         BIT_LOCATION_RTCALM_ALMEN               (0b1 << 6)
#define         RTCALM_FUNC_ALARM_GLOBAL_DISABLE        (0b0 << 6)
#define         RTCALM_FUNC_ALARM_GLOBAL_ENABLE         (0b1 << 6)

#define         RTCALM_FUNC_YEAR_ALARM_ENABLE           (0b1 << 5)
#define         RTCALM_FUNC_YEAR_ALARM_DISABLE          (0b0 << 5)

#define         RTCALM_FUNC_MONTH_ALARM_ENABLE          (0b1 << 4)
#define         RTCALM_FUNC_MONTH_ALARM_DISABLE         (0b0 << 4)

#define         RTCALM_FUNC_DAY_ALARM_ENABLE            (0b1 << 3)
#define         RTCALM_FUNC_DAY_ALARM_DISABLE           (0b0 << 3)

#define         RTCALM_FUNC_HOUR_ALARM_ENABLE           (0b1 << 2)
#define         RTCALM_FUNC_HOUR_ALARM_DISABLE          (0b0 << 2)

#define         RTCALM_FUNC_MINUTE_ALARM_ENABLE         (0b1 << 1)
#define         RTCALM_FUNC_MINUTE_ALARM_DISABLE        (0b0 << 1)

#define         RTCALM_FUNC_SEC_ALARM_ENABLE            (0b1 << 0)
#define         RTCALM_FUNC_SEC_ALARM_DISABLE           (0b0 << 0)

//You can clear specific bits of INTP register by writing 1’s to the bits that you want to clear regardless of RTCEN value.
#define         BIT_LOCATION_INTP_ALARM_INTERRUPT       (0b1 << 1)
#define         INTP_FUNC_ALARM_INTERRUPT_OCCURRED      (0b1 << 1)

//函数功能：把十进制 num 转成 bcd 码，譬如把 56 转成 0x56
static unsigned int num_2_bcd(unsigned int num)
{
    //第一步，把56拆分成 5 和 6
    //第二步，把 5 和 6组合成 0x56
    return  ( ((num / 10) << 4) | (num % 10));
}

//函数功能：把 bcd 码bcd,转成十进制,譬如把 0x56 转成 56
static unsigned int bcd_2_num(unsigned int bcd)
{
    //第一步，把 0x56 拆分成 5 和 6
    //第二步，把 5 和 6 组合成56
    return  ( ((bcd & 0xf0) >> 4) * 10 + (bcd & 0xf));
}

void rtc_set_time(const struct rtc_time *p)
{
   //第一步，打开 RTC 读写开关 
   rRTCCON &= ~(BIT_LOCATION_RTCCON_RTCEN_CONTROL);
   rRTCCON |= (RTCCON_FUNC_RTC_CONTROL_ENABLE);

   //第二步，写 RTC 时间寄存器
   rBCDYEAR = num_2_bcd(p->year - 2000);
   rBCDMON =  num_2_bcd(p->month);
   rBCDDATE=  num_2_bcd(p->date);
   rBCDHOUR = num_2_bcd(p->hour);
   rBCDMIN =  num_2_bcd(p->minute);
   rBCDSEC =  num_2_bcd(p->second);
   rBCDDAY =  num_2_bcd(p->day);

   //最后一个，关上 RTC 的读写开关
   rRTCCON &= ~(BIT_LOCATION_RTCCON_RTCEN_CONTROL);
   rRTCCON |= (RTCCON_FUNC_RTC_CONTROL_DISABLE);

}

void rtc_get_time(struct rtc_time *p)
{
   //第一步，打开 RTC 读写开关 
   rRTCCON &= ~(BIT_LOCATION_RTCCON_RTCEN_CONTROL);
   rRTCCON |= (RTCCON_FUNC_RTC_CONTROL_ENABLE);

   //第二步，读 RTC 时间寄存器
   p->year    = bcd_2_num(rBCDYEAR) + 2000;  
   p->month   = bcd_2_num(rBCDMON);  
   p->date    = bcd_2_num(rBCDDATE);  
   p->hour    = bcd_2_num(rBCDHOUR); 
   p->minute  = bcd_2_num(rBCDMIN); 
   p->second  = bcd_2_num(rBCDSEC); 
   p->day     = bcd_2_num(rBCDDAY); 

   //最后一个，关上 RTC 的读写开关
   rRTCCON &= ~(BIT_LOCATION_RTCCON_RTCEN_CONTROL);
   rRTCCON |= (RTCCON_FUNC_RTC_CONTROL_DISABLE);
}

//函数功能：实现 秒级别的闹钟
void rtc_set_alarm(void)
{
    rALMSEC = num_2_bcd(31);

    rRTCALM &= ~(BIT_LOCATION_RTCALM_ALMEN);
    rRTCALM |= (RTCALM_FUNC_ALARM_GLOBAL_ENABLE);

    rRTCALM |= (RTCALM_FUNC_SEC_ALARM_ENABLE);
}

void isr_rtc_alarm(void)
{
    static int i = 0;
    printf("rtc alarm, i = %d...\r\n", i++);

    rINTP |= (INTP_FUNC_ALARM_INTERRUPT_OCCURRED);
    intc_clearVectaddr();
}
