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

_start:
        //第0步：开发板置锁
        ldr r0, =0xe010e81c
        ldr r1, [r0]
        orr r1, r1, #0x300
        orr r1, r1, #0x1
        str r1, [r0]

	// 长跳转到 main(led 闪烁) 开始第二阶段 
	ldr pc, =main      // ldr 指令实现长挑战
	
	
// 汇编最后的这个死循环不能丢
	b .
	

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

