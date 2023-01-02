// 定义操作寄存器的宏
#define GPH0CON		0xE0200C00
#define GPH0DAT		0xE0200C04

#define GPH2CON		0xE0200C40
#define GPH2DAT		0xE0200C44

#define rGPH0CON	(*(volatile unsigned int *)GPH0CON)
#define rGPH0DAT	(*(volatile unsigned int *)GPH0DAT)
#define rGPH2CON	(*(volatile unsigned int *)GPH2CON)
#define rGPH2DAT	(*(volatile unsigned int *)GPH2DAT)


/************************************************************************************/
#define BIT_WIDTH_GPH0_CON              (4)

#define BIT_LOCATION_GPH0_2_FUNC        (0xf << 2 * BIT_WIDTH_GPH0_CON)
#define GPH0_2_FUNC_INPUT               (0x0 << 2 * BIT_WIDTH_GPH0_CON)
#define GPH0_2_FUNC_OUTPUT              (0x1 << 2 * BIT_WIDTH_GPH0_CON)
#define GPH0_2_FUNC_EXT_INT2            (0Xf << 2 * BIT_WIDTH_GPH0_CON)

#define BIT_LOCATION_GPH0_3_FUNC        (0xf << 3 * BIT_WIDTH_GPH0_CON)
#define GPH0_3_FUNC_INPUT               (0x0 << 3 * BIT_WIDTH_GPH0_CON)
#define GPH0_3_FUNC_OUTPUT              (0x1 << 3 * BIT_WIDTH_GPH0_CON)
#define GPH0_3_FUNC_EXT_INT3            (0Xf << 3 * BIT_WIDTH_GPH0_CON)


/************************************************************************************/
#define BIT_WIDTH_GPH2_CON              (4)


#define BIT_LOCATION_GPH2_0_FUNC        (0xf << 0 * BIT_WIDTH_GPH2_CON)
#define GPH2_0_FUNC_INPUT               (0x0 << 0 * BIT_WIDTH_GPH2_CON)
#define GPH2_0_FUNC_OUTPUT              (0x1 << 0 * BIT_WIDTH_GPH2_CON)
#define GPH2_0_FUNC_EXT_INT16           (0Xf << 0 * BIT_WIDTH_GPH2_CON)

#define BIT_LOCATION_GPH2_1_FUNC        (0xf << 1 * BIT_WIDTH_GPH2_CON)
#define GPH2_1_FUNC_INPUT               (0x0 << 1 * BIT_WIDTH_GPH2_CON)
#define GPH2_1_FUNC_OUTPUT              (0x1 << 1 * BIT_WIDTH_GPH2_CON)
#define GPH2_1_FUNC_EXT_INT17           (0Xf << 1 * BIT_WIDTH_GPH2_CON)

#define BIT_LOCATION_GPH2_2_FUNC        (0xf << 2 * BIT_WIDTH_GPH2_CON)
#define GPH2_2_FUNC_INPUT               (0x0 << 2 * BIT_WIDTH_GPH2_CON)
#define GPH2_2_FUNC_OUTPUT              (0x1 << 2 * BIT_WIDTH_GPH2_CON)
#define GPH2_2_FUNC_EXT_INT18           (0Xf << 2 * BIT_WIDTH_GPH2_CON)

#define BIT_LOCATION_GPH2_3_FUNC        (0xf << 3 * BIT_WIDTH_GPH2_CON)
#define GPH2_3_FUNC_INPUT               (0x0 << 3 * BIT_WIDTH_GPH2_CON)
#define GPH2_3_FUNC_OUTPUT              (0x1 << 3 * BIT_WIDTH_GPH2_CON)
#define GPH2_3_FUNC_EXT_INT19           (0Xf << 3 * BIT_WIDTH_GPH2_CON)

// 初始化按键
void key_init(void)
{
    // 设置GPHxCON寄存器，设置为输入模式
    // GPH0CON的bit8～15全部设置为0，即可
    rGPH0CON &= ~(BIT_LOCATION_GPH0_2_FUNC | BIT_LOCATION_GPH0_3_FUNC);
    // GPH2CON的bit0～15全部设置为0，即可
    rGPH2CON &= ~(BIT_LOCATION_GPH2_0_FUNC | BIT_LOCATION_GPH2_1_FUNC |
		  BIT_LOCATION_GPH2_2_FUNC | BIT_LOCATION_GPH2_3_FUNC);
}

void delay_ms(unsigned int m_seconds)
{
    // 这个函数作用是延时20ms左右
    // 因为我们这里是裸机程序，且重点不是真的要消抖，而是教学
    // 所以我这里这个程序只是象征性的，并没有实体
    // 如果是研发，那就要花时间真的调试出延时20ms的程序
    unsigned int i, j;
    unsigned int sum = m_seconds * 1000;

    for (i=0; i< sum; i++)
    {
        for (j=0; j<1000; j++)
        {  
	    i * j;
        }
    }
}

void key_polling(void)
{
    // 依次，挨个去读出每个GPIO的值，判断其值为1还是0.如果为1则按键按下，为0则弹起
    // 轮询的意思就是反复循环判断有无按键，隔很短时间
    while (1)
    {
        // 对应开发板上标着LEFT的那个按键
        if (!(rGPH0DAT & (1<<2)))		
        {
	    delay_ms(20);
            if (!(rGPH0DAT & (1<<2)))
            {
	        printf("KEY LEFT press!\r\n");
	    }
        }

        // 对应开发板上标着DOWN的那个按键
        if (!(rGPH0DAT & (1<<3)))
        {
	    delay_ms(20);
            if (!(rGPH0DAT & (1<<3)))
	    {
		printf("KEY DOWN press!\r\n");
            }
	}
        		
        // 对应开发板上标着UP的那个按键
        if (!(rGPH2DAT & (1<<0)))
        {
	    delay_ms(20);
            if (!(rGPH2DAT & (1<<0)))
	    {
		printf("KEY UP press!\r\n");
	    }
        }

	// 对应开发板上标着RIGHT的那个按键
        if (!(rGPH2DAT & (1<<1)))
	{
	    delay_ms(20);
            if (!(rGPH2DAT & (1<<1)))
	    {
		printf("KEY RIGHT press!\r\n");
	    }
	}

        // 对应开发板上标着BACK的那个按键
        if (!(rGPH2DAT & (1<<2)))
	{
	    delay_ms(20);
            if (!(rGPH2DAT & (1<<2)))
	    {
	        printf("KEY BACK press!\r\n");
	    }
	}

	// 对应开发板上标着MENU的那个按键
        if (!(rGPH2DAT & (1<<3)))
	{ 
	    delay_ms(20);
            if (!(rGPH2DAT & (1<<3)))
	    {
		printf("KEY MENU press!\r\n");
	    }
	}
    }

}
  
