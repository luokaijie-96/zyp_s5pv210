// 命令解析和命令执行相关的函数
#include "shell.h"
#include "stdio.h"
#include "main.h"
#include "ctype.h"
#include "lib/vsprintf.h"

char g_cmdset[CMD_NUM][MAX_LINE_LENGTH];		// 命令集，存主命令
char cmd[MAX_CMD_PART][MAX_LEN_PART];			// 当前解析出来的命令
int cmd_index = -1;								// 存储解析到的命令是第几个主命令

/****************************具体硬件操作命令处理函数*************************/

void cmd_test()
{
	int i = 0;
	for(i=0; i < 5; i++)
	{
		printf("cmd[%d] -- %s\r\n", i, cmd[i]);
	}
}

void printf_separator()
{
	printf("=============================>\r\n");
}

// 命令没找到处理方法
void do_cmd_notfound(void)
{
	printf("%s Command Error!\r\n", cmd[0]);
	printf_separator();
}

void inline led_usage(void)
{
	printf("LED Usage:\r\n"
	       "    led on\r\n"
		   "    led off\r\n"
		   "    led blink [counts]\r\n");
}


// led命令的处理方法
void do_cmd_led(void)
{
	//puts("led cmd");
	// 真正的led命令的操作实现
	// 目前支持的命令有led on | led off 
	// cmd[0]里面是led，cmd[1]里面是on|off
	if (!strcmp(cmd[1], "on"))
	{
		// led on
		led_on();
	}
	else if (!strcmp(cmd[1], "off"))
	{
		// led off
		led_off();
	}
	else if (!strcmp(cmd[1], "blink"))
	{
		if (!strcmp(cmd[2], ""))
		{
			led_blink_counts(1);
		}
		else if(isdigit(cmd[2]))
		{
			led_usage();
			printf_separator();
		}
		else
		{
			int cnt = simple_strtoul(cmd[2], cmd[2] + MAX_LEN_PART, 10);
			led_blink_counts(cnt);
		}
	}
	// ..... 还可以继续扩展
	else
	{
		// 如果一个都没匹配，则打印使用方法
		do_cmd_notfound();
		led_usage();
		printf_separator();
	}
}

void inline buzzer_usage(void)
{
	printf("BUZZER Usage:\r\n"
	       "    buzzer on\r\n"
		   "    buzzer off\r\n"
		   "    buzzer freq 10000\r\n"
		   "    buzzer freq 2000\r\n"
		   "    buzzer freq 100\r\n");
}

// 蜂鸣器命令处理方法
void do_cmd_buzzer(void)
{
	//puts("led cmd");
	// 真正的buzzer命令的操作实现
	// 目前支持的命令有buzzer on | buzzer off 
	// cmd[0]里面是buzzer，cmd[1]里面是on|off
	if (!strcmp(cmd[1], "on"))
	{
		// buzzer on
		buzzer_on();
	}
	else if (!strcmp(cmd[1], "off"))
	{
		// buzzer off
		buzzer_off();
	}
	else if (!strcmp(cmd[1], "freq"))
	{
		if ((!strcmp(cmd[2], "")) || (isdigit(cmd[2])))
		{
			do_cmd_notfound();
			buzzer_usage();
			printf_separator();
		}
		else
		{
			int freq = simple_strtoul(cmd[2], cmd[2] + MAX_LEN_PART, 10);
			if (freq != 10000 && freq != 2000 && freq != 100)
			{
				buzzer_usage();
				printf_separator();
			}
			else
			{
				buzzer_set_freq(freq);
			}
		}
	}
	// ..... 还可以继续扩展
	else
	{
		// 如果一个都没匹配，则打印使用方法
		do_cmd_notfound();
		buzzer_usage();
		printf_separator();
	}
}

// lcd命令处理方法


// ADC命令处理方法
void inline adc_usage(void)
{
	printf("ADC Usage:\r\n"
		   "    adc test [counts]\r\n");
}

void do_cmd_adc(void)
{
	// 真正的adc命令的操作实现
	if (!strcmp(cmd[1], "test"))
	{
		if ((!strcmp(cmd[2], "")))
		{
			adc_collect1(1);
		}
		else if (isdigit(cmd[2]))
		{
            do_cmd_notfound();
            adc_usage();
			printf_separator();
		}
		else
		{
			int counts = simple_strtoul(cmd[2], cmd[2] + MAX_LEN_PART, 10);
			adc_collect1(counts);
		}
		
	}
	// ..... 还可以继续扩展
	else
	{
		// 如果一个都没匹配，则打印使用方法
		do_cmd_notfound();
		adc_usage();
		printf_separator();
	}
}




/*********************************shell 命令解析执行框架***********************/

// 初始化命令列表
void init_cmd_set(void)
{
	memset((char *)g_cmdset, 0, sizeof(g_cmdset));		// 先全部清零
	strcpy(g_cmdset[0], led);
	strcpy(g_cmdset[1], lcd);
	strcpy(g_cmdset[2], pwm);
	strcpy(g_cmdset[3], adc);
	
	memset((char *)cmd, 0, sizeof(cmd));	
}

// 将用户输入的字符串命令str按照空格分隔成多个字符串，依次放入cmd二维数组中
void cmdsplit(char cmd[][MAX_LEN_PART], const char *str)
{
    int m = 0, n = 0;	// m表示二位数组第一维，n表示第二维
    while (*str != '\0')
    {
    	if (*str != ' ')
    	{
    		cmd[m][n] = *str;
    		n++;
    	}
    	else
    	{
    		cmd[m][n] = '\0';
    		n = 0;
    		m++;
    	}
    	str++;
    }
    cmd[m][n] = '\0';
}

// 解析命令
void cmd_parser(char *str)
{
	int i;

	memset((char *)cmd, 0, sizeof(cmd));
	
    // 第一步，先将用户输入的次命令字符串分割放入cmd中
    cmdsplit(cmd, str);
    
	cmd_test();
    // 第二步，将cmd中的次命令第一个字符串和cmdset对比
    cmd_index = -1;
    for (i=0; i<CMD_NUM; i++)
    {
    	// cmd[0]就是次命令中的第一个字符串，也就是主命令
    	if (!strcmp(cmd[0], g_cmdset[i]))
    	{
    		// 相等，找到了这个命令，就去执行这个命令所对应的动作。
    		//puts("您输入的命令是：");
    		//puts(str);
    		//puts("\n");
    		cmd_index = i;
    		
    		break;
    	}
    }		
}


// 执行命令
void cmd_exec(void)
{
	switch (cmd_index)
	{
		case 0:		// led
			do_cmd_led();			break;
		case 1:		// lcd
			//do_cmd_lcd();			break;
		case 2:		// buzzer	
			do_cmd_buzzer();		break;
		case 3:		// buzzer	
			do_cmd_adc();			break;
		default:
			do_cmd_notfound();		
			led_usage();
			buzzer_usage();
			adc_usage(); 
			printf_separator();
			break;
	}
}
