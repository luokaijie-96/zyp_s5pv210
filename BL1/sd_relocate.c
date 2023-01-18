#define  SD_START_BLOCK     (45)
#define  SD_BLOCK_CNT       (32)
#define  DDR_START_ADDR     (0x23E00000)

typedef unsigned int bool;

//通道号: 0, 或者 2
//开始扇区号: 45
//读取扇区个数: 32
//读取后放入内存地址: 0x23E00000
//with_init:0
typedef  bool (*pCopySDMMC2Mem)(int, unsigned int, unsigned short, unsigned int*, bool);
//#define CopySDMMCtoMem(z,a,b,c,e)(((bool(*)(int, unsigned int, unsigned short, unsigned int*, bool))(*((unsigned int *)0xD0037F98)))(z,a,b,c,e))


typedef  void (*pBL2Type)(void);

//从SD卡第45扇区开始, 复制 32 个扇区内容到 DDR 的 0x23E00000, 然后跳转到 0x23E00000 去执行.
void copy_bl2_2_ddr(void)
{
    //第一步,读取 SD 卡扇区到 DDR 中
    pCopySDMMC2Mem p1 = (pCopySDMMC2Mem)(*(unsigned int*)0xD0037F98);

    p1(2, SD_START_BLOCK, SD_BLOCK_CNT, (unsigned int*)DDR_START_ADDR, 0); //读取 SD 卡到 DDR 中

    //第二步,跳转到 DDR 中的 BL2 去执行
    pBL2Type p2 = (pBL2Type)DDR_START_ADDR;
    p2();
}
