#define GPA0CON		0xE0200000
#define UCON0 		0xE2900004
#define ULCON0 		0xE2900000
#define UMCON0 		0xE290000C
#define UFCON0 		0xE2900008
#define UBRDIV0 	0xE2900028
#define UDIVSLOT0	0xE290002C
#define UTRSTAT0	0xE2900010
#define UTXH0		0xE2900020	
#define URXH0		0xE2900024	

#define rGPA0CON       	(*(volatile unsigned int *)GPA0CON)
#define rUCON0		    (*(volatile unsigned int *)UCON0)
#define rULCON0		    (*(volatile unsigned int *)ULCON0)
#define rUMCON0		    (*(volatile unsigned int *)UMCON0)
#define rUFCON0		    (*(volatile unsigned int *)UFCON0)
#define rUBRDIV0	    (*(volatile unsigned int *)UBRDIV0)
#define rUDIVSLOT0	    (*(volatile unsigned int *)UDIVSLOT0)
#define rUTRSTAT0       (*(volatile unsigned int *)UTRSTAT0)
#define rUTXH0		    (*(volatile unsigned int *)UTXH0)
#define rURXH0		    (*(volatile unsigned int *)URXH0)

/**************************************************************************************/
#define ULCON0_FUNC_NO_PARITY_MODE             (0b000 << 3)  // 无校验
#define ULCON0_FUNC_NUMBER_STOP_0_BIT          (0b0   << 2)  // Stop bit: 0
#define ULCON0_FUNC_WORLD_LENGTH_8_BIT         (0b11  << 0)  // 8 位数据位

/**************************************************************************************/
#define UCON0_FUNC_CLOCK_SELECTION_PLCK        (0b0   <<10)  //选择PCLK
#define UCON0_FUNC_LOOPBACK_MODE_NORMAL        (0b0   << 5)  //正常模式
#define UCON0_FUNC_TX_MODE_POLLING             (0b01  << 2)  //轮询模式
#define UCON0_FUNC_RX_MODE_POLLING             (0b01  << 0)  //轮询模式

/**************************************************************************************/
#define UFCON0_FUNC_FIFO_DISABLE               (0b0   << 0)  //禁止 FIFO 模式

/**************************************************************************************/
#define UTRSTAT0_FUNC_TRANSMITTER_EMPTY                       (0b1   << 2)  //发送缓冲区为空
#define BIT_LOCATION_UTRSTAT0_FUNC_TRANSMITTER_EMPTY          (0b1   << 2)  //发送缓冲器状态位
#define UTRSTAT0_FUNC_RECEIVE_BUFFER_DATA_READY               (0b1   << 0)  //接收缓冲区已接收到数据
#define BIT_LOCATION_UTRSTAT0_FUNC_RECEIVE_BUFFER_DATA_READY  (0b1   << 0)  //接收缓冲区状态位

/**************************************************************************************/
#define BIT_WIDTH_GPA0                  (4)   
#define GPA0_0_FUNC_INPUT               (0x0 << 0 * BIT_WIDTH_GPA0)
#define GPA0_0_FUNC_OUTPUT              (0x1 << 0 * BIT_WIDTH_GPA0)
#define GPA0_0_FUNC_UART0RXD            (0x2 << 0 * BIT_WIDTH_GPA0)
#define GPA0_0_FUNC_GPA0_INT0           (0Xf << 0 * BIT_WIDTH_GPA0)
							            
#define GPA0_1_FUNC_INPUT               (0x0 << 1 * BIT_WIDTH_GPA0)
#define GPA0_1_FUNC_OUTPUT              (0x1 << 1 * BIT_WIDTH_GPA0)
#define GPA0_1_FUNC_UART0TXD            (0x2 << 1 * BIT_WIDTH_GPA0)
#define GPA0_1_FUNC_GPA0_INT1           (0Xf << 1 * BIT_WIDTH_GPA0)

#define BIT_LOCATION_GPA0_CON0          (0xf << 0 * BIT_WIDTH_GPA0)
#define BIT_LOCATION_GPA0_CON1          (0xf << 1 * BIT_WIDTH_GPA0)

/**************************************************************************************/

//串口初始化程序
void uart_init(void)
{
    // 初始化 TX RX 对应的 GPIO 引脚
    rGPA0CON &= ~(BIT_LOCATION_GPA0_CON0 | BIT_LOCATION_GPA0_CON1);  
    rGPA0CON |= GPA0_0_FUNC_UART0RXD  | GPA0_1_FUNC_UART0TXD;

    //几个关键寄存器的设置
    rULCON0 = ULCON0_FUNC_NO_PARITY_MODE | ULCON0_FUNC_NUMBER_STOP_0_BIT | ULCON0_FUNC_WORLD_LENGTH_8_BIT;
    rUCON0 = UCON0_FUNC_CLOCK_SELECTION_PLCK | UCON0_FUNC_LOOPBACK_MODE_NORMAL | UCON0_FUNC_TX_MODE_POLLING | UCON0_FUNC_RX_MODE_POLLING;
    rUMCON0 = 0;
    rUFCON0 = UFCON0_FUNC_FIFO_DISABLE;
#if 0
    //波特率设置  DIV_VAL = (PCLK / (bps X 16) ) - 1
    //PCLK_PSYS 用 66MHz 算  DIV_VAL = (66 X 10^6 / (115200 X 16)) - 1 = 34.807
    //小数是 0.8，0.8 x 16 = 12.8
    rUBRDIV0 = 34;
    //12 0xDDDD(1101_1101_1101_1101b)
    //13 0xDFDD(1101_1111_1101_1101b)
    rUDIVSLOT0 =  0xDDDD;
#endif

    //PCLK_PSYS 用 66.7MHz 算  DIV_VAL = (66.7 X 10^6 / (115200 X 16)) - 1 = 35.187
    //小数是 0.18，0.18 x 16 = 2.88
    rUBRDIV0 = 35;
    //2 0x0808(0000_1000_0000_1000b)
    //3 0x0888(0000_1000_1000_1000b)
    rUDIVSLOT0 =  0x0808;
}

//串口接收程序，轮询方式，接收一个字节
void uart_putc(char c)
{
    //串口发送一个字符，其实就是把一个字节丢到发送缓冲区中去
    //因为串口控制器发送 1 个字节的速度远远低于 CPU 的速度，所以 CPU 发送1个字节前必须
    //确认串口控制器当前缓冲区是空的（意思就是串口已经发完了上一个字节）
    //如果缓冲区非空则位为0，此时应该循环，直到位为1
    while ((rUTRSTAT0 & BIT_LOCATION_UTRSTAT0_FUNC_TRANSMITTER_EMPTY) != UTRSTAT0_FUNC_TRANSMITTER_EMPTY) ;
    rUTXH0 = c;
}

//串口接收程序，轮询方式，接收一个字节
char uart_getc(void)
{
    while ((rUTRSTAT0 & BIT_LOCATION_UTRSTAT0_FUNC_RECEIVE_BUFFER_DATA_READY) != UTRSTAT0_FUNC_RECEIVE_BUFFER_DATA_READY) ;
    return  rURXH0 & 0xff;
}
