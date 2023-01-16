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


objs := start.o led.o clock.o uart.o main.o init.o key.o pwm.o wdt.o 
objs += lib/libc.a

wdt.bin: $(objs)
	$(LD) -Tlink.lds -o wdt.elf $^
	$(OBJCOPY) -O binary wdt.elf wdt.bin
	$(OBJDUMP) -D wdt.elf > wdt_elf.dis
	gcc mkv210_image.c -o mkx210
	./mkx210 wdt.bin 210.bin

lib/libc.a:
	cd lib;	make;	cd ..
	
%.o : %.S
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< -c

%.o : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< -c

clean:
	rm *.o *.elf *.bin *.dis mkx210 -f
	cd lib; make clean; cd ..

	
	

