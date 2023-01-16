#ifndef __RTC_H__
#define __RTC_H__

struct rtc_time
{
    unsigned int year;
    unsigned int month;
    unsigned int date;    //日期几号
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
    unsigned int day;     //星期几
};

void rtc_get_time(struct rtc_time *p);
void rtc_set_time(const struct rtc_time *p);
void isr_rtc_alarm(void);
void rtc_set_alarm(void);
#endif
