#include "main.h"
#include "ascii.h"
#include "1024600.h"

#define GPF0CON			(*(volatile unsigned long *)0xE0200120)
#define GPF1CON			(*(volatile unsigned long *)0xE0200140)
#define GPF2CON			(*(volatile unsigned long *)0xE0200160)
#define GPF3CON			(*(volatile unsigned long *)0xE0200180)

#define GPD0CON			(*(volatile unsigned long *)0xE02000A0)
#define GPD0DAT			(*(volatile unsigned long *)0xE02000A4)

#define CLK_SRC1		(*(volatile unsigned long *)0xe0100204)
#define CLK_DIV1		(*(volatile unsigned long *)0xe0100304)
#define DISPLAY_CONTROL	(*(volatile unsigned long *)0xe0107008)

#define VIDCON0			(*(volatile unsigned long *)0xF8000000)
#define VIDCON1			(*(volatile unsigned long *)0xF8000004)
#define VIDTCON2		(*(volatile unsigned long *)0xF8000018)
#define WINCON0 		(*(volatile unsigned long *)0xF8000020)
#define WINCON2 		(*(volatile unsigned long *)0xF8000028)
#define SHADOWCON 		(*(volatile unsigned long *)0xF8000034)
#define VIDOSD0A 		(*(volatile unsigned long *)0xF8000040)
#define VIDOSD0B 		(*(volatile unsigned long *)0xF8000044)
#define VIDOSD0C 		(*(volatile unsigned long *)0xF8000048)

#define VIDW00ADD0B0 	(*(volatile unsigned long *)0xF80000A0)
#define VIDW00ADD1B0 	(*(volatile unsigned long *)0xF80000D0)

#define VIDTCON0 		(*(volatile unsigned long *)0xF8000010)
#define VIDTCON1 		(*(volatile unsigned long *)0xF8000014)

#define HSPW 			(40)				// 1~40 DCLK
#define HBPD			(10 - 1)			// 46
#define HFPD 			(240 - 1)			// 16 210 354
#define VSPW			(20)				// 1~20 DCLK
#define VBPD 			(10 - 1)			// 23
#define VFPD 			(30 - 1)			// 7 22 147



// FB地址1024*600
#define FB_ADDR			(0x23000000)
#define ROW			(600)//(480)
#define COL			(1024)//(800)
#define HOZVAL			(COL-1)
#define LINEVAL			(ROW-1)

#define XSIZE			COL
#define YSIZE			ROW

u32 *pfb = (u32 *)FB_ADDR;

// 常用颜色定义
#define BLACK	0x000000
#define BLUE	0x0000FF
#define RED	0xFF0000
#define GREEN	0x00FF00
#define WHITE	0xFFFFFF
#define  LightPink       0xFFB6C1        //浅粉红
#define  Pink            0xFFC0CB        //粉红
#define  Crimson         0xDC143C        //猩红
#define  LavenderBlush   0xFFF0F5        //脸红的淡紫色
#define  PaleVioletRed   0xDB7093        //苍白的紫罗兰红色
#define  HotPink         0xFF69B4        //热情的粉红
#define  DeepPink        0xFF1493        //深粉色
#define  MediumVioletRed 0xC71585        //适中的紫罗兰红色
#define  Orchid          0xDA70D6        //兰花的紫色
#define  Thistle         0xD8BFD8        //蓟
#define  Plum            0xDDA0DD        //李子
#define  Violet          0xEE82EE        //紫罗兰
#define  Magenta         0xFF00FF        //洋红
#define  Fuchsia         0xFF00FF        //灯笼海棠(紫红色)
#define  DarkMagenta     0x8B008B        //深洋红色
#define  Purple          0x800080        //紫色
#define  MediumOrchid    0xBA55D3        //适中的兰花紫
#define  DarkViolet      0x9400D3        //深紫罗兰色
#define  DarkOrchid      0x9932CC        //深兰花紫
#define  Indigo          0x4B0082        //靛青
#define  BlueViolet      0x8A2BE2        //深紫罗兰的蓝色
#define  MediumPurple    0x9370DB        //适中的紫色
#define  MediumSlateBlue 0x7B68EE        //适中的板岩暗蓝灰色
#define  SlateBlue       0x6A5ACD        //板岩暗蓝灰色
#define  DarkSlateBlue   0x483D8B        //深板岩暗蓝灰色
#define  Lavender        0xE6E6FA        //薰衣草花的淡紫色
#define  GhostWhite      0xF8F8FF        //幽灵的白色
#define  Blue            0x0000FF        //纯蓝
#define  IndigoBlue      0x0000FF        //靛蓝
#define  MediumBlue      0x0000CD        //适中的蓝色
#define  MidnightBlue    0x191970        //午夜的蓝色
#define  DarkBlue        0x00008B        //深蓝色
#define  Navy            0x000080        //海军蓝,深靛蓝
#define  RoyalBlue       0x4169E1        //皇家蓝
#define  CornflowerBlue  0x6495ED        //矢车菊的蓝色
#define  LightSteelBlue  0xB0C4DE        //淡钢蓝
#define  LightSlateGray  0x778899        //浅石板灰
#define  SlateGray       0x708090        //石板灰
#define  DodgerBlue      0x1E90FF        //道奇蓝
#define  AliceBlue       0xF0F8FF        //爱丽丝蓝
#define  SteelBlue       0x4682B4        //钢蓝

static void lcd_draw_background(u32 color);

// 初始化LCD
void lcd_init(void)
{
	// 配置引脚用于LCD功能
	GPF0CON = 0x22222222;
	GPF1CON = 0x22222222;
	GPF2CON = 0x22222222;
	GPF3CON = 0x22222222;

	// 打开背光	GPD0_0（PWMTOUT0）
	GPD0CON &= ~(0xf<<0);
	GPD0CON |= (1<<0);			// output mode
	GPD0DAT &= ~(1<<0);			// output 0 to enable backlight

	// 10: RGB=FIMD I80=FIMD ITU=FIMD
	DISPLAY_CONTROL = 2<<0;

	// bit[26~28]:使用RGB接口
	// bit[18]:RGB 并行
	// bit[2]:选择时钟源为HCLK_DSYS=166MHz
	VIDCON0 &= ~( (3<<26)|(1<<18)|(1<<2) );

	// bit[1]:使能lcd控制器
	// bit[0]:当前帧结束后使能lcd控制器
	VIDCON0 |= ( (1<<0)|(1<<1) );

	// bit[6]:选择需要分频
	// bit[6~13]:分频系数为5，即VCLK = 166M/(4+1) = 33M
	VIDCON0 |= 4<<6 | 1<<4;


	// H43-HSD043I9W1.pdf(p13) 时序图：VSYNC和HSYNC都是低脉冲
	// s5pv210芯片手册(p1207) 时序图：VSYNC和HSYNC都是高脉冲有效，所以需要反转
	VIDCON1 |= 1<<5 | 1<<6;

	// 设置时序
	VIDTCON0 = VBPD<<16 | VFPD<<8 | VSPW<<0;
	VIDTCON1 = HBPD<<16 | HFPD<<8 | HSPW<<0;
	// 设置长宽(物理屏幕)
	VIDTCON2 = (LINEVAL << 11) | (HOZVAL << 0);

	// 设置window0
	// bit[0]:使能
	// bit[2~5]:24bpp（RGB888）
	WINCON0 |= 1<<0;
	WINCON0 &= ~(0xf << 2);
	WINCON0 |= (0xB<<2) | (1<<15);

#define LeftTopX     0
#define LeftTopY     0
#define RightBotX   1023//799
#define RightBotY   599//479

	// 设置window0的上下左右
	// 设置的是显存空间的大小
	VIDOSD0A = (LeftTopX<<11) | (LeftTopY << 0);
	VIDOSD0B = (RightBotX<<11) | (RightBotY << 0);
	VIDOSD0C = (LINEVAL + 1) * (HOZVAL + 1);


	// 设置fb的地址
	VIDW00ADD0B0 = FB_ADDR;
	VIDW00ADD1B0 = (((HOZVAL + 1)*4 + 0) * (LINEVAL + 1)) & (0xffffff);

	// 使能channel 0传输数据
	SHADOWCON = 0x1;


	lcd_draw_background(DarkViolet);
}

// 在像素点(x, y)处填充为color颜色
static inline void lcd_draw_pixel(u32 x, u32 y, u32 color)
{
	*(pfb + COL * y + x) = color;
}

// 把整个屏幕全部填充成一个颜色color
static void lcd_draw_background(u32 color)
{
	u32 i, j;
	
	for (j=0; j<ROW; j++)
	{
		for (i=0; i<COL; i++)
		{
			lcd_draw_pixel(i, j, color);
		}
	}
}

static void delay(void)
{
	volatile u32 i, j;
	for (i=0; i<4000; i++)
		for (j=0; j<100; j++);
}


// 绘制横线，起始坐标为(x1, y)到(x2, y),颜色是color
static void lcd_draw_hline(u32 x1, u32 x2, u32 y, u32 color)
{
	u32 x;
	
	for (x = x1; x<x2; x++)
	{
		lcd_draw_pixel(x, y, color);
	}
}

// 绘制竖线，起始坐标为(x, y1)到(x, y2),颜色是color
static void lcd_draw_vline(u32 x, u32 y1, u32 y2, u32 color)
{
	u32 y;
	
	for (y = y1; y<y2; y++)
	{
		lcd_draw_pixel(x, y, color);
	}
}

// glib库中的画线函数，可以画斜线，线两端分别是(x1, y1)和(x2, y2)
void glib_line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color)
{
	int dx,dy,e;
	dx=x2-x1; 
	dy=y2-y1;
    
	if(dx>=0)
	{
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 1/8 octant
			{
				e=dy-dx/2;  
				while(x1<=x2)
				{
					lcd_draw_pixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else		// 2/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					lcd_draw_pixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 8/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					lcd_draw_pixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else	 // 7/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					lcd_draw_pixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
	else //dx<0
	{
		dx=-dx;		//dx=abs(dx)
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 4/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					lcd_draw_pixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 3/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					lcd_draw_pixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 5/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					lcd_draw_pixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 6/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					lcd_draw_pixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
}

//画圆函数，圆心坐标是(centerX, centerY)，半径是radius，圆的颜色是color
void draw_circular(unsigned int centerX, unsigned int centerY, unsigned int radius, unsigned int color)
{
	int x,y ;
	int tempX,tempY;;
    int SquareOfR = radius*radius;

	for(y=0; y<XSIZE; y++)
	{
		for(x=0; x<YSIZE; x++)
		{
			if(y<=centerY && x<=centerX)
			{
				tempY=centerY-y;
				tempX=centerX-x;                        
			}
			else if(y<=centerY&& x>=centerX)
			{
				tempY=centerY-y;
				tempX=x-centerX;                        
			}
			else if(y>=centerY&& x<=centerX)
			{
				tempY=y-centerY;
				tempX=centerX-x;                        
			}
			else
			{
				tempY = y-centerY;
				tempX = x-centerX;
			}
			if ((tempY*tempY+tempX*tempX)<=SquareOfR)
				lcd_draw_pixel(x, y, color);
		}
	}
}



// 写字
// 写字的左上角坐标(x, y)，字的颜色是color，字的字模信息存储在data中
static void show_8_16(unsigned int x, unsigned int y, unsigned int color, unsigned char *data)  
{  
    // count记录当前正在绘制的像素的次序
    int i, j, count = 0;  
	  
    for (j=y; j<(y+16); j++)  
    {  
        for (i=x; i<(x+8); i++)  
        {  
            if (i<XSIZE && j<YSIZE)  
            {  
		    // 在坐标(i, j)这个像素处判断是0还是1，如果是1写color；如果是0直接跳过
		    if (data[count/8] & (1<<(count%8)))   
			    lcd_draw_pixel(i, j, color);
            }  
            count++;  
        }  
    }  
} 

// 写字符串
// 字符串起始坐标左上角为(x, y)，字符串文字颜色是color,字符串内容为str
void draw_ascii_ok(unsigned int x, unsigned int y, unsigned int color, unsigned char *str)
{
    int i;  
    unsigned char *ch;
    for (i=0; str[i]!='\0'; i++)  
    {  
	    ch = (unsigned char *)ascii_8_16[(unsigned char)str[i]-0x20];
	    show_8_16(x, y, color, ch); 

	    x += 8;
	    if (x >= XSIZE)
	    {
		    x -= XSIZE;			// 回车
		    y += 16;			// 换行
	    }
    }  
}


// 画1024x600的图，图像数据存储在pData所指向的数组中
void lcd_draw_picture(const unsigned char *pData)
{
	u32 x, y, color, p = 0;

	for (y=0; y<YSIZE; y++)
	{
		for (x=0; x<XSIZE; x++)
		{
			// 在这里将坐标点(x, y)的那个像素填充上相应的颜色值即可
			color = (pData[p+0] << 0) | (pData[p+1] << 8) | (pData[p+2] << 16);
			lcd_draw_pixel(x, y, color);
			p += 3;
		}
	}
}

void lcd_test(void)
{
	//lcd_init();
	
#if 0
        // 测试绘制背景色，成功
	while (1)
	{
		lcd_draw_background(MidnightBlue);
                delay();

                lcd_draw_background(DarkViolet);
                delay();

                lcd_draw_background(Fuchsia);
		delay();
	}
#endif


#if 0	
	// 测试画斜线
	lcd_draw_background(DarkViolet);                // 如果没有加这一行，背景色是花花绿绿的 
	glib_line(0, LINEVAL, HOZVAL, 0, RED);
        draw_circular(HOZVAL/2, LINEVAL/2, 50, GREEN);



	// 测试画横线、竖线
        lcd_draw_background(DarkViolet);
        lcd_draw_hline(HOZVAL/2 - 100, HOZVAL/2 + 100, LINEVAL/2, RED);
        lcd_draw_vline(HOZVAL/2, LINEVAL/2 - 50, LINEVAL/2 + 50, GREEN);
#endif

	// 测试写英文字母
	
	draw_ascii_ok(0, 0, RED, "ABCDabcd1234!@#$%");

	//lcd_draw_picture(gImage_1024600);

}


