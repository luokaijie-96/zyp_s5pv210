#include "stdio.h"
#include "main.h"
#include "init.h"

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

/************************************************************************************/
//定义外部中断寄存器的宏

#define EXT_INT_0_CON	(0xE0200E00)
#define EXT_INT_2_CON	(0xE0200E08)
#define EXT_INT_0_PEND	(0xE0200F40)
#define EXT_INT_2_PEND	(0xE0200F48)
#define EXT_INT_0_MASK	(0xE0200F00)
#define EXT_INT_2_MASK	(0xE0200F08)

#define rEXT_INT_0_CON  (*(volatile unsigned int *)EXT_INT_0_CON)
#define rEXT_INT_2_CON  (*(volatile unsigned int *)EXT_INT_2_CON)
#define rEXT_INT_0_PEND (*(volatile unsigned int *)EXT_INT_0_PEND)
#define rEXT_INT_2_PEND (*(volatile unsigned int *)EXT_INT_2_PEND)
#define rEXT_INT_0_MASK	(*(volatile unsigned int *)EXT_INT_0_MASK)
#define rEXT_INT_2_MASK	(*(volatile unsigned int *)EXT_INT_2_MASK)


// 每 4 个 bit 为一组，最高 bit 位是 reserved bit
#define EXT_INT_CON_FUNC_LOW_LEVEL                (0b000)
#define EXT_INT_CON_FUNC_HIGH_LEVEL               (0b001)
#define EXT_INT_CON_FUNC_FALLING_EDGE_TRIGGER     (0b010)
#define EXT_INT_CON_FUNC_RISING_EDGE_TRIGGER      (0b011)
#define EXT_INT_CON_FUNC_BOTH_EDGE_TRIGGER        (0b100)

#define EXT_INT_MASK_FUNC_ENABLE_INTERRUPT        (0b0)
#define EXT_INT_MASK_FUNC_MASKED                  (0b1)

#define EXT_INT_PEND_FUNC_NOT_OCCUR               (0b0)
#define EXT_INT_PEND_FUNC_OCCUR_INTERRUPT         (0b1)

/************************************************************************************/



/******************************  轮询方式处理按键  **********************************/
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
    volatile unsigned int i, j;
    volatile unsigned int sum = m_seconds * 2;

    for (i=0; i< sum; i++)
    {
        for (j=0; j<1000; j++)
        {
            i * j;
        }
    }
}


void delay_seconds(unsigned int seconds)
{
    // 这个函数作用是延时20ms左右
    // 因为我们这里是裸机程序，且重点不是真的要消抖，而是教学
    // 所以我这里这个程序只是象征性的，并没有实体
    // 如果是研发，那就要花时间真的调试出延时20ms的程序
    volatile unsigned int i, j;
    volatile unsigned int sum = seconds * 1500;

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
  

/******************************  轮询方式处理按键  **********************************/
//以中断方式来处理按键的初始化
void key_init_interrupt(void)
{
    //1.外部中断对应的 GPIO 模式设置
    rGPH0CON &= ~(BIT_LOCATION_GPH0_2_FUNC | BIT_LOCATION_GPH0_3_FUNC);
    rGPH2CON &= ~(BIT_LOCATION_GPH2_0_FUNC | BIT_LOCATION_GPH2_1_FUNC |
		  BIT_LOCATION_GPH2_2_FUNC | BIT_LOCATION_GPH2_3_FUNC);

    rGPH0CON |= (GPH0_2_FUNC_EXT_INT2 | GPH0_3_FUNC_EXT_INT3);
    rGPH2CON |= (GPH2_0_FUNC_EXT_INT16 | GPH2_1_FUNC_EXT_INT17 
		|GPH2_2_FUNC_EXT_INT18 | GPH2_3_FUNC_EXT_INT19);


    //2.中断触发模式设置
    rEXT_INT_0_CON &= ~((0xf << (2 * 4)) | (0xf << (3 * 4))); //GPH0_2 / GPH0_3 清除设置
    rEXT_INT_0_CON |= ( (EXT_INT_CON_FUNC_FALLING_EDGE_TRIGGER << (2 * 4)) 
		      | (EXT_INT_CON_FUNC_FALLING_EDGE_TRIGGER << (3 * 4)) );

    rEXT_INT_2_CON &= ~( (0xf << (0 * 4)) | (0xf << (1 * 4))
		       | (0xf << (2 * 4)) | (0xf << (3 * 4)) ); //GPH2_0~GPH2_3 清除设置
    rEXT_INT_2_CON |= ( (EXT_INT_CON_FUNC_FALLING_EDGE_TRIGGER << (0 * 4))
		      | (EXT_INT_CON_FUNC_FALLING_EDGE_TRIGGER << (1 * 4)) 
		      | (EXT_INT_CON_FUNC_FALLING_EDGE_TRIGGER << (2 * 4))
		      | (EXT_INT_CON_FUNC_FALLING_EDGE_TRIGGER << (3 * 4)) );

    //3. 外部中断允许
    rEXT_INT_0_MASK &= ~( (0X1 << 2) | (0X1 << 3) );
    rEXT_INT_0_MASK |= ( (EXT_INT_MASK_FUNC_ENABLE_INTERRUPT << 2) 
		       | (EXT_INT_MASK_FUNC_ENABLE_INTERRUPT << 3) ); 

    rEXT_INT_2_MASK &= ~( (0X1 << 0) | (0X1 << 1) | (0X1 << 2) | (0X1 << 3) );
    rEXT_INT_2_MASK |= ( (EXT_INT_MASK_FUNC_ENABLE_INTERRUPT << 0)
		       | (EXT_INT_MASK_FUNC_ENABLE_INTERRUPT << 1)
		       | (EXT_INT_MASK_FUNC_ENABLE_INTERRUPT << 2) 
		       | (EXT_INT_MASK_FUNC_ENABLE_INTERRUPT << 3) ); 

    //4.清挂起,清除是写1，不是写0
    rEXT_INT_0_PEND |= (3 << 2);
    rEXT_INT_2_PEND |= (0xf << 0);

}

//EINT2 通道对应的按键,就是 GPH0_2 引脚对应的按键, 就是开发板上标了 LEFT 的那个按键
void isr_eint2(void)
{
    //真正的 isr 应该做 2 件事情
    //第一, 中断处理代码，就是真正干活的代码
    printf("isr_eint2 LEFT.\r\n");
    //第二,清除中断挂起
    //printf("init: rEXT_INT_0_PEND [%p]\r\n.", rEXT_INT_0_PEND);
    //rEXT_INT_0_PEND &= ~(0X1 << 2);
    //printf("init: rEXT_INT_0_PEND [%p]\r\n.", rEXT_INT_0_PEND);
    rEXT_INT_0_PEND |= (0X1 << 2);
    //printf("init: rEXT_INT_0_PEND [%p]\r\n.", rEXT_INT_0_PEND);
    intc_clearVectaddr();
}

void isr_eint3(void)
{
    //真正的 isr 应该做 2 件事情
    //第一, 中断处理代码，就是真正干活的代码
    printf("isr_eint3 DOWN.\r\n");
    //第二,清除中断挂起
    //printf("init: rEXT_INT_0_PEND [%p]\r\n.", rEXT_INT_0_PEND);
    //rEXT_INT_0_PEND &= ~(0X1 << 3);
    //printf("init: rEXT_INT_0_PEND [%p]\r\n.", rEXT_INT_0_PEND);
    rEXT_INT_0_PEND |= (0X1 << 3);
    //printf("init: rEXT_INT_0_PEND [%p]\r\n.", rEXT_INT_0_PEND);
    intc_clearVectaddr();
}

void isr_eint16171819(void)
{
    //真正的 isr 应该做 2 件事情
    //第一, 中断处理代码，就是真正干活的代码
    //因为 EINT16~32 是共享中断,所以要在这里再次去区分具体是哪个子中断
    if (rEXT_INT_2_PEND & ( 0x1 << 0))
    {
        printf("isr_eint16.\r\n");	
    }
    else if (rEXT_INT_2_PEND & ( 0x1 << 1))
    {
        printf("isr_eint17.\r\n");	
    }
    else if (rEXT_INT_2_PEND & ( 0x1 << 2))
    {
        printf("isr_eint18.\r\n");	
    }
    else if (rEXT_INT_2_PEND & ( 0x1 << 3))
    {
        printf("isr_eint19.\r\n");	
    }
    //第二,清除中断挂起
    rEXT_INT_2_PEND |= (0Xf << 0);
    intc_clearVectaddr();
}

