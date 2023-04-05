#ifndef __MAIN_H__
#define __MAIN_H__

typedef unsigned int u32;
typedef unsigned short u16;

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

int is_keyboard_press(void);

//pwm.c
void timer2_pwm_init(void);
void buzzer_on(void);
void buzzer_off(void);
void buzzer_set_freq(unsigned int frep);


//adc.c
void adc_init();
void adc_test(void);
void adc_collect1(unsigned int counts);

//lcd.c
void lcd_init();
void lcd_test(void);


//wdt.c
void wdt_init_interrupt(void);
void wdt_timer_init();

//stdio.c
void putchar(char c);
void puts(const char *p);
char getchar(void);
char *gets(char *p);

#endif
