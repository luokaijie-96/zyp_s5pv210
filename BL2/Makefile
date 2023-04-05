CC		= arm-linux-gcc
LD 		= arm-linux-ld
OBJCOPY	= arm-linux-objcopy
OBJDUMP	= arm-linux-objdump
AR		= arm-linux-ar

INCDIR	:= $(shell pwd)
# C PreProcesser flag
CPPFLAGS	:= -nostdlib -nostdinc -I$(INCDIR)/include
# C Compiler flag
CFLAGS		:= -Wall -O2 -fno-builtin

# export these variables for lib/Makefile  to use
export CC LD OBJCOPY OBJDUMP AR CPPFLAGS CFLAGS


objs := start.o led.o clock.o uart.o main.o init.o key.o pwm.o wdt.o rtc.o adc.o
objs += stdio.o cmd.o lcd.o env.o
objs += lib/libc.a

BL2.bin: $(objs)
	$(LD) -Tlink.lds -o BL2.elf $^
	$(OBJCOPY) -O binary BL2.elf BL2.bin
	$(OBJDUMP) -D BL2.elf > BL2_elf.dis

lib/libc.a:
	cd lib;	make;	cd ..
	
%.o : %.S
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< -c

%.o : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< -c

clean:
	rm *.o *.elf *.bin *.dis mkx210 -f
	cd lib; make clean; cd ..

	
	

