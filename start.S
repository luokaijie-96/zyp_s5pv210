/*
 * 文件名: led.S
 * 作者: 罗凯杰
 * 描述: 演示重定位(在 SRAM 内部重定位)
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

	
	//第 2 步, 设置 SVC 栈(复位之后, 自动进入 SVC 模式)
	ldr sp, =SVC_STACK


	//第 3 步, 开/关 icache
	mrc p15, 0, r0, c1, c0, 0;    // 读出 cp15 的 c1 到 r0 中
	bic r0, r0, #(1 << 12)        // bit 12 置0 关 icache
	orr r0, r0, #(1 << 12)        // bit 12 置1 开 icache
	mcr p15, 0, r0, c1, c0, 0;



	//第 4 步, 重定位
	//adr 指令用于加载_start 当前运行地址
	adr  r0, _start     //adr 加载时就叫做: 短加载
	//ldr 指令用于加载_start 的链接地址: 0xd0024000
	ldr  r1, =_start    //ldr 加载时如果目标寄存器是pc就叫长跳转,如果目标寄存器是r1等就叫长加载
	//bss 段的起始地址
	ldr  r2, =bss_start   //就是我们重定位代码的结束地址, 重定位只需重定位代码段和数据段即可
	cmp  r0, r1           //比较_start 的运行时地址和链接地址是否相等
        beq clean_bss	      //如果相等说明不需要重定位,所以跳过 copy_loop，直接跳到 clean_bss
			      //如果不相等说明需要重定位,那么直接执行下面的 copy_loop 进行重定位
			      //重定位完成后继续执行 clean_bss


	//用汇编来实现的一个 while 循环
copy_loop:
	ldr  r3, [r0], #4    //源
	str  r3, [r1], #4    //目的 这两句代码就完成了 4 个字节内容的拷贝
	cmp  r1, r2          // r1 和 r2 都是用 ldr 加载的, 都是链接地址，所以 r1 不断+4 总能等于 r2
	bne  copy_loop



	//清 bss 段, 其实就是在链接地址处把 bss 段全部清零
clean_bss:
	ldr  r0, =bss_start
	ldr  r1, =bss_end
	cmp  r0, r1          //如果 r0 等于 r1, 说明 bss 段为空,直接下去
	beq  run_on_dram     //清除 bss 完之后的地址
	mov  r2, #0
clear_loop:
	str  r2, [r0], #4    //先将 r2 中的值放入 r0 所指向的内存地址(r0 的值作为内存地址)
	cmp  r0, r1          //然后 r0 = r0 + 4
	bne  clear_loop


run_on_dram:
	//长跳转到 led_blink 开始第二阶段
	ldr  pc, =led_blink   //ldr 指令实现长跳转


	//从这里之后, 就可以开始调用 C 程序了
        //bl  led_blink      // bl 指令实现短跳转

//汇编最后的这个死循环不能丢
	b   .
	

