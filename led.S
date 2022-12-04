/*
 * 文件名: led.S
 * 作者: 罗凯杰
 * 描述: 演示汇编关闭看门狗  
 */

#define GPJ0CON 0xE0200240
#define GPJ0DAT 0xE0200244 

#define WTCON   0xE2700000

.global _start  //解决 make 编译警告: arm-linux-ld: warning: cannot find entry symbol _start; defaulting to 00000000
                // 把 _start 链接属性改为外部，这样其他文件就可以看见 _start 了
_start:
	//第 1 步,关看门狗(向 WTCON 的 bit5 写入 0 即可)
	ldr r0, =WTCON
	ldr r1, =0x0
	str r1, [r0]
	
	

	//之后的为功能代码
	//第一步: 把所有引脚设置成输出模式 
	ldr r0, =0x11111111     //从后面的 = 可以看出用的是 ldr 伪指令，因为需要编译器来判断这个数
	ldr r1, =GPJ0CON        //是合法立即数还是非法立即数，一般写代码都用 ldr 伪指令
	str r0, [r1]            // 寄存器间接寻址。功能是把 r0 中的数写入到 r1 中的数为地址的内存中去

	//要实现流水灯，只要在主循环中实现 1 圈的流水显示效果即可
flash:
	//第 1 步: 点亮 LED1， 其他熄灭
	ldr r0, =~(1 << 3) 
	ldr r1, =GPJ0DAT
	str r0, [r1]            //把 0 写入到 GPJ0DAT 寄存器中，引脚即输出低电平，LED 点亮
	//然后延时
	bl  delay               // 使用 bl 进行函数调用


	//第 2 步: 点亮 LED2， 其他熄灭
	ldr r0, =~(1 << 4) 
	ldr r1, =GPJ0DAT
	str r0, [r1]            //把 0 写入到 GPJ0DAT 寄存器中，引脚即输出低电平，LED 点亮
	//然后延时
	bl  delay               // 使用 bl 进行函数调用


	//第 3 步: 点亮 LED3， 其他熄灭
	ldr r0, =~(1 << 5) 
	ldr r1, =GPJ0DAT
	str r0, [r1]            //把 0 写入到 GPJ0DAT 寄存器中，引脚即输出低电平，LED 点亮
	//然后延时
	bl  delay               // 使用 bl 进行函数调用

	
	b flash


	b .                     // . 代表当前这一句指令的地址，这个就是高大上的死循环




//延时函数
delay:
	ldr r2, =10000000
	ldr r3, =0x0
delay_loop:
	sub r2, r2, #1          // r2 = r2 -1
	cmp r2, r3              // cmp 会影响 z 标志位，如果 r2 等于 r3 则 z =1，下一句中 eq 就会成立
	bne delay_loop
	mov pc, lr              // 函数调用返回


