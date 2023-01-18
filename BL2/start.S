
/*
 * 文件名：	start.s	
 * 作者：       罗凯杰	
 * 描述：	演示 BL2 的代码
 */

#define WTCON		0xE2700000

#define SVC_STACK	0xd0037d80

.global _start					// 把_start链接属性改为外部，这样其他文件就可以看见_start了
_start:
	// 长跳转到 main(led 闪烁) 开始第二阶段 
	ldr pc, =main      // ldr 指令实现长挑战
	
	
// 汇编最后的这个死循环不能丢
	b .
	

	
	
	

	
	
	
	
	
	
	
	
	
