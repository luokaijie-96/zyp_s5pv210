#ifndef __MAIN_H__
#define __MAIN_H__

//main.h 就是用来存放各个外设的操作函数的声明的.

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



#endif
