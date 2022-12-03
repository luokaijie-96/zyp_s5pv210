/*
 * 文件名: led.S
 * 作者: 罗凯杰
 * 描述: 这是我们一步步点亮 LED 教程的自己写的第一个裸机程序
 */

#define GPJ0CON 0xE0200240
#define GPJ0DAT 0xE0200244 

.global _start  //解决 make 编译警告: arm-linux-ld: warning: cannot find entry symbol _start; defaulting to 00000000
                // 把 _start 链接属性改为外部，这样其他文件就可以看见 _start 了
_start:
	//第一步: 把所有引脚设置成输出模式 
	ldr r0, =0x11111111     //从后面的 = 可以看出用的是 ldr 伪指令，因为需要编译器来判断这个数
	ldr r1, =GPJ0CON        //是合法立即数还是非法立即数，一般写代码都用 ldr 伪指令
	str r0, [r1]            // 寄存器间接寻址。功能是把 r0 中的数写入到 r1 中的数为地址的内存中去

	//第二步: 把中间 LED (GPJ0_4) 的输出设置为 0，其余两颗设置为 1，剩下的其他位不管 
	ldr r0, =((1 << 3) | (0 << 4) | (1 << 5))
	ldr r1, =GPJ0DAT
	str r0, [r1]            //把 0 写入到 GPJ0DAT 寄存器中，引脚即输出低电平，LED 点亮


	b .                     // . 代表当前这一句指令的地址，这个就是高大上的死循环






