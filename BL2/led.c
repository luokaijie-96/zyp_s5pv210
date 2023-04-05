#define GPJ0CON     0xE0200240
#define GPJ0DAT     0xE0200244 

#define rGPJ0CON    *((volatile unsigned int*)GPJ0CON)
#define rGPJ0DAT    *((volatile unsigned int*)GPJ0DAT) 

static void delay(void);

//该函数要实现 led 闪烁效果
void led_blink(void)
{
    //led 初始化，也就是把 GPJ0CON 中设置为输出模式
    //unsigned int* p   =  (unsigned int*)GPJ0CON;
    //unsigned int* p1  =  (unsigned int*)GPJ0DAT;

    //*p = 0x11111111;
    rGPJ0CON = 0x11111111;

    while (1)
    {
        rGPJ0DAT = (0 << 3) | (0 << 4) | (0 << 5);
        delay();        
        rGPJ0DAT = (1 << 3) | (1 << 4) | (1 << 5);
        delay();
    }
}

void led_blink_counts(unsigned int cnt)
{
    //led 初始化，也就是把 GPJ0CON 中设置为输出模式
    //unsigned int* p   =  (unsigned int*)GPJ0CON;
    //unsigned int* p1  =  (unsigned int*)GPJ0DAT;

    //*p = 0x11111111;
    rGPJ0CON = 0x11111111;

    while (cnt--)
    {
        rGPJ0DAT ^= (0b111 << 3);
        delay();        
        rGPJ0DAT ^= (0b111 << 3);
        delay();
    }
}

// led初始化
void led_init()
{
	rGPJ0CON = 0x11111111;
	rGPJ0DAT = ((0<<3) | (0<<4) | (1<<5));
}	

void led_on(void)
{
	rGPJ0DAT = ((0<<3) | (0<<4) | (0<<5));
}

void led_off(void)
{
	rGPJ0DAT = ((1<<3) | (1<<4) | (1<<5));
}

static void delay(void)
{
    volatile unsigned int i = 1000000;  // volatile 让编译器不要优化，这样才能真正的减
    while (i--);                        //才能消耗时间, 实现 delay
}

