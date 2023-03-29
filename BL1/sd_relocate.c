#define  SD_START_BLOCK     (45)
//#define  SD_BLOCK_CNT       (32)  //ÕâÀïÖ»ÄÜ¿½±´ 16kB µÄ BL2
#define  SD_BLOCK_CNT       (4096)   //»­Í¼ÏóµÄBL2,´óĞ¡Îª 1MB ×óÓÒ,ËùÒÔ±ØĞëÔö´ó¿½±´µÄÉÈÇøÊı
#define  DDR_START_ADDR     (0x23E00000)

typedef unsigned int bool;

// Í¨µÀºÅ£º0£¬»òÕß2
// ¿ªÊ¼ÉÈÇøºÅ£º45
// ¶ÁÈ¡ÉÈÇø¸öÊı£º32
// ¶ÁÈ¡ºó·ÅÈëÄÚ´æµØÖ·£º0x23E00000
// with_init£º0
typedef  bool (*pCopySDMMC2Mem)(int, unsigned int, unsigned short, unsigned int*, bool);
//#define CopySDMMCtoMem(z,a,b,c,e)(((bool(*)(int, unsigned int, unsigned short, unsigned int*, bool))(*((unsigned int *)0xD0037F98)))(z,a,b,c,e))


typedef  void (*pBL2Type)(void);

// ´ÓSD¿¨µÚ45ÉÈÇø¿ªÊ¼£¬¸´ÖÆ32¸öÉÈÇøÄÚÈİµ½DDRµÄ0x23E00000£¬È»ºóÌø×ªµ½23E00000È¥Ö´ĞĞ
void copy_bl2_2_ddr(void)
{
    // µÚÒ»²½£¬¶ÁÈ¡SD¿¨ÉÈÇøµ½DDRÖĞ	
    pCopySDMMC2Mem p1 = (pCopySDMMC2Mem)(*(unsigned int*)0xD0037F98);

    // ¶ÁÈ¡SD¿¨µ½DDRÖĞ
    p1(2, SD_START_BLOCK, SD_BLOCK_CNT, (unsigned int*)DDR_START_ADDR, 0); //è¯»å– SD å¡åˆ° DDR ä¸­

    // µÚ¶ş²½£¬Ìø×ªµ½DDRÖĞµÄBL2È¥Ö´ĞĞ
    pBL2Type p2 = (pBL2Type)DDR_START_ADDR;
    p2();
}
