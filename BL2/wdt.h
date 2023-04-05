#ifndef __WDT_H__
#define __WDT_H__

void isr_wdt(void);
void wdt_init_interrupt(void);
unsigned int pow(unsigned int base, unsigned int exponent);

#endif
