/*
 * 文件名: led.S
 * 作者: 罗凯杰
 * 描述: 演示汇编初始化时钟系统
 */


#define WTCON       0xE2700000

#define SVC_STACK   0xD0037D80  //满减栈

.global _start  //解决 make 编译警告: arm-linux-ld: warning: cannot find entry symbol _start; defaulting to 00000000
                // 把 _start 链接属性改为外部，这样其他文件就可以看见 _start 了
_start:
	//第 1 步,关看门狗(向 WTCON 的 bit5 写入 0 即可)
	ldr r0, =WTCON
	ldr r1, =0x0
	str r1, [r0]

	// 第2步：初始化时钟
	bl clock_init
	
	//第 3 步, 设置 SVC 栈(复位之后, 自动进入 SVC 模式)
	ldr sp, =SVC_STACK


	//第 4 步, 开/关 icache
	mrc p15, 0, r0, c1, c0, 0;    // 读出 cp15 的 c1 到 r0 中
	bic r0, r0, #(1 << 12)        // bit 12 置0 关 icache
	orr r0, r0, #(1 << 12)        // bit 12 置1 开 icache
	mcr p15, 0, r0, c1, c0, 0;


	//从这里之后, 就可以开始调用 C 程序了
        bl  led_blink

//汇编最后的这个死循环不能丢
	b   .
	

