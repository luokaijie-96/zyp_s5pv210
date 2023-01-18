/*
 * 文件名: start.S
 * 作者: 罗凯杰
 * 描述: 演示中断入口函数的汇编代码 
 */


#define WTCON       0xE2700000

#define SVC_STACK   0xD0037D80  //满减栈
#define IRQ_STACK   0xD0037F80  //满减栈


.global _start  //解决 make 编译警告: arm-linux-ld: warning: cannot find entry symbol _start; defaulting to 00000000
.global IRQ_handle

                // 把 _start 链接属性改为外部，这样其他文件就可以看见 _start 了
_start:
        //第0步：开发板置锁
        ldr r0, =0xe010e81c
        ldr r1, [r0]
        orr r1, r1, #0x300
        orr r1, r1, #0x1
        str r1, [r0]        

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

        bl main

	//从这里之后, 就可以开始调用 C 程序了
        //bl  led_blink

//汇编最后的这个死循环不能丢
	b   .
	

//在这个汇编函数中, 用来做中断模式下的现场保护和恢复,并且调用真正的中断处理程序

IRQ_handle:
    //设置 IRQ 模式下的栈
    ldr sp, =IRQ_STACK
    //保存 LR
    //因为 ARM 有流水线, 所以 PC 的值会比真正执行的代码 +8
    sub lr, lr, #4
    //保存 r0-r12 和 lr 到 irq 模式下的栈上面
    stmfd sp!, {r0-r12, lr}
    //在此调用真正的 isr 来处理中断
    bl irq_handler
    //处理完成开始恢复现场, 其实就是做中断返回, 关键是将 r0-r12，pc, cpsr 一起恢复
    ldmfd sp!, {r0-r12, pc}^

