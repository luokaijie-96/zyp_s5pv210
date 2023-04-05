#ifndef __MAIN_H__
#define __MAIN_H__

//main.h 就是用来存放各个外设的操作函数的声明的.

//led.c
void led_init();
void led_on(void);
void led_off(void);
void led_blink_counts(unsigned int cnt);


//key.c
void key_init(void);
void key_polling(void);
void delay_seconds(unsigned int seconds);
void delay_ms(unsigned int m_seconds);
 
void key_init_interrupt(void);
void isr_eint2(void);
void isr_eint3(void);
void isr_eint16171819(void);

//uart.c
void uart_init(void);
void putc(unsigned char c);
unsigned char getc(void);

//pwm.c
void timer2_pwm_init(void);
void buzzer_on(void);
void buzzer_off(void);
void buzzer_set_freq(unsigned int frep);


//adc.c
void adc_init();
void adc_test(void);
void adc_collect1(unsigned int counts);

#endif
