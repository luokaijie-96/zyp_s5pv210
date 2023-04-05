#include "init.h"
#include "stdio.h"

void reset_exception(void)
{
    printf("reset_exception.\r\n");
}
void undef_exception(void)
{
    printf("undef_exception.\r\n");
}
void soft_init_exception(void)
{
    printf("soft_init_exception.\r\n");
}
void prefetch_exception(void)
{
    printf("prefetch_exception.\r\n");
}
void data_exception(void)
{
    printf("data_exception.\r\n");
}

//主要功能: 绑定第一阶段异常向量表;禁止所有中断;选择所有中断类型为 IRQ
//清除 VICnADDR 为 0
void system_init_exception(void)
{
    //第一阶段处理, 绑定异常向量表
    r_exception_reset =      (unsigned int)reset_exception;
    r_exception_undef =      (unsigned int)undef_exception;
    r_exception_soft_init =  (unsigned int)soft_init_exception;
    r_exception_prefetch =   (unsigned int)prefetch_exception;
    r_exception_data =       (unsigned int)data_exception;
    r_exception_irq =        (unsigned int)IRQ_handle;
    r_exception_fiq =        (unsigned int)IRQ_handle;

    //初始化中断控制器的基本寄存器
    intc_init();

}

//清除需要处理的中断的中断处理函数的地址
void intc_clearVectaddr(void)
{
    //VICxADDR : 当前正在处理的中断的中断处理函数的地址
    VIC0ADDR = 0;
    VIC1ADDR = 0;
    VIC2ADDR = 0;
    VIC3ADDR = 0;
}

//初始化中断控制器
void intc_init(void)
{
    //禁止所有中断.
    //为什么在中断初始化之初要禁止所有中断?
    //因为中断一旦打开,因为外部或者硬件自己的原因产生中断后一定就会寻找 isr
    //而我们可能认为自己用不到这个中断就没有提供 isr，这时它自动拿到的就是乱码
    //则程序很可能跑飞，所以不用的中断一定要关掉.
    //一般的做法是先全部关掉,然后再逐一打开自己感兴趣的中断.
    //一旦打开就必须给这个中断提供相应的 isr 并绑定好.
    VIC0INTENCLEAR = 0xffffffff;
    VIC1INTENCLEAR = 0xffffffff;
    VIC2INTENCLEAR = 0xffffffff;
    VIC3INTENCLEAR = 0xffffffff;

    //选择中断类型为 IRQ
    VIC0INTSELECT  = 0x0;
    VIC1INTSELECT  = 0x0;
    VIC2INTSELECT  = 0x0;
    VIC3INTSELECT  = 0x0;

    //清 VICxADDR 
    intc_clearVectaddr();

}
	
//绑定我们写的 isr 到 VICnVECTADDR 寄存器
//绑定过之后我们就把 isr 地址交给硬件了,剩下的我们不用管了,硬件自己会处理
//等发生相应中断的时候,我们直接到相应的 VICnADDR 中去取 isr 地址即可.
//参数: intNum 是 init.h 定义的物理中断号,handler 是函数指针,就是我们写的 isr

//VIC0VECTADDR 定义为 VIC0VECTADDR0 寄存器的地址,就相当于是 VIC0VECTADDR0~31 这个
//数组(这个数组就是一个函数指针数组)的首地址,然后具体计算每一个中断的时候
//只需要首地址+偏移量即可.
void intc_setVectaddr(unsigned long intNum, void (*handler)(void))
{
    //VIC0
    if (intNum < 32)
    {
	*( (volatile unsigned long *)(VIC0VECTADDR + (4 * (intNum - 0)))) =  (unsigned long)handler;
    }
    else if (intNum < 64) // VIC1
    {
	*( (volatile unsigned long *)(VIC1VECTADDR + (4 * (intNum - 32)))) = (unsigned long)handler;
    }
    else if (intNum < 96) // VIC2
    {
	*( (volatile unsigned long *)(VIC2VECTADDR + (4 * (intNum - 64)))) = (unsigned long)handler;
    }
    else //VIC3
    {
	*( (volatile unsigned long *)(VIC3VECTADDR + (4 * (intNum - 96)))) = (unsigned long)handler;
    }
}

// 使能中断
// 通过传参的intnum来使能某个具体的中断源，中断号在int.h中定义，是物理中断号
void intc_enable(unsigned long intnum)
{
    unsigned long temp;
        // 确定intnum在哪个寄存器的哪一位
        // <32就是0～31，必然在VIC0
    if(intnum<32)
    {
        //temp = VIC0INTENABLE;
        //temp |= (1<<intnum);          // 如果是第一种设计则必须位操作，第二种设计可以
                                                                        // 直接写。
        VIC0INTENABLE |= (0x1 << (intnum - 0));
    }
    else if(intnum<64)
    {
        //temp = VIC1INTENABLE;
        //temp |= (1<<(intnum-32));
        //VIC1INTENABLE = temp;
        VIC1INTENABLE |= (0x1 << (intnum - 32));
    }
    else if(intnum<96)
    {
        //temp = VIC2INTENABLE;
        //temp |= (1<<(intnum-64));
        //VIC2INTENABLE = temp;
        VIC2INTENABLE |= (0x1 << (intnum - 64));
    }
    else if(intnum<NUM_ALL)
    {
        //temp = VIC3INTENABLE;
        //temp |= (1<<(intnum-96));
        //VIC3INTENABLE = temp;
        VIC3INTENABLE |= (0x1 << (intnum - 96));
    }
    // NUM_ALL : enable all interrupt
    else
    {
        VIC0INTENABLE = 0xFFFFFFFF;
        VIC1INTENABLE = 0xFFFFFFFF;
        VIC2INTENABLE = 0xFFFFFFFF;
        VIC3INTENABLE = 0xFFFFFFFF;
    }

}


// 禁止中断
// 通过传参的intnum来禁止某个具体的中断源，中断号在int.h中定义，是物理中断号
void intc_disable(unsigned long intnum)
{
    unsigned long temp;

    if(intnum<32)
    {
        //temp = VIC0INTENCLEAR;
        //temp |= (1<<intnum);
        //VIC0INTENCLEAR = temp;
        VIC0INTENCLEAR |= (0x1 << (intnum - 0));
    }
    else if(intnum<64)
    {
        //temp = VIC1INTENCLEAR;
        //temp |= (1<<(intnum-32));
        //VIC1INTENCLEAR = temp;
        VIC1INTENCLEAR |= (0x1 << (intnum - 32));
    }
    else if(intnum<96)
    {
        //temp = VIC2INTENCLEAR;
        //temp |= (1<<(intnum-64));
        //VIC2INTENCLEAR = temp;
        VIC2INTENCLEAR |= (0x1 << (intnum - 64));
    }
    else if(intnum<NUM_ALL)
    {
        //temp = VIC3INTENCLEAR;
        //temp |= (1<<(intnum-96));
        //VIC3INTENCLEAR = temp;
        VIC3INTENCLEAR |= (0x1 << (intnum - 96));
    }
    // NUM_ALL : disable all interrupt
    else
    {
        VIC0INTENCLEAR = 0xFFFFFFFF;
        VIC1INTENCLEAR = 0xFFFFFFFF;
        VIC2INTENCLEAR = 0xFFFFFFFF;
        VIC3INTENCLEAR = 0xFFFFFFFF;
    }

    return;
}


//通过读取 VICnIRQSTATUS 寄存器，判断其中有哪个有 1 bit 为 1，来得知哪个 VIC 发生中断了
unsigned long intc_getVICirqstatus(unsigned long ucontroller)
{
    if (ucontroller == 0)
    {
	return VIC0IRQSTATUS;
    }
    else if (ucontroller == 1)
    {
	return VIC1IRQSTATUS;
    }
    else if (ucontroller == 2)
    {
	return VIC2IRQSTATUS;
    }
    else if (ucontroller == 3)
    {
	return VIC3IRQSTATUS;
    }
    else
    {
	return 0;
    }
}

//真正的中断处理程序.意思就是说这里只考虑中断处理, 不考虑保护/恢复现场
void irq_handler(void)
{
    //printf("irq_handler.\r\n");	
    //SoC 支持很多个(在低端 CPU 例如 2440 中有30 多个,在 210中有100多个)中断
    //这么多中断 irq 在第一个阶段走的是一条路,都会进入到 irq_handler 来
    //我们在 irq_handler 中要去区分究竟是哪个中断发生了,然后再去调用该中断对应的 isr
    
    //虽然硬件已经自动帮我们把 isr 放入了 VICnADDR 中，但是因为有 4 个, 所以我们必须
    //先去软件检查出来到底哪个 VIC 中断了, 也就是说 isr 到底在哪个 VICnADDR 寄存器中
    unsigned long vicaddr[4] = {VIC0ADDR, VIC1ADDR, VIC2ADDR, VIC3ADDR};

    int i = 0;
    void (*isr)(void) = NULL;

    for( i = 0; i < 4; i++)
    {
	//发生一个中断时, 4 个 VIC 中有 3 个是全 0,有 1 个的其中 1 bit 不是 0
	if (intc_getVICirqstatus(i) != 0)
	{
	    isr = (void (*)(void)) vicaddr[i];
	    break;
	}
    }

    (*isr)(); //通过函数指针来调用函数
}
