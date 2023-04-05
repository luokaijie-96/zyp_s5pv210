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
		//printf("cmd[%d] -- %s\r\n", i, cmd[i]);
	}
}

void printf_separator()
{
	printf("=============================>\r\n");
}


void print_help()
{
	printf_separator();
	led_usage();
    lcd_usage();
    buzzer_usage();
    adc_usage(); 
	printenv_usage();
	setenv_usage();
    printf_separator();
}


/******************************环境变量处理函数****************************/
void inline printenv_usage(void)
{
	printf("printenv   --- print all environments values.\r\n");
}

void do_cmd_printenv(void)
{
	int i;
	// 先找目前有没有这个环境变量，如果有就直接改值
	for (i=0; i<MAX_ENV_NUM; i++)
	{
		if (envset[i].is_used)
		{
			// 打印出来的环境变量格式是：bootdelay=3
			printf("%s=%s\r\n", envset[i].env_name, envset[i].env_val);
		}
	}
}


void inline setenv_usage(void)
{
	printf("setenv ENV_NAME           --- delete environments values.\r\n"
		   "setenv ENV_NAME ENV_VAL   --- set environments values.\r\n");
}

void do_cmd_setenv(void)
{
	// 目前支持的命令有setenv envname envval 
	// cmd[0]里面是setenv，cmd[1]里面是envname cmd[2]里面是envval
	if (!strcmp(cmd[1], ""))
	{
		// 如果cmd[1]是null，说明用户用法不对
		do_cmd_notfound();
		setenv_usage();
		printf_separator();
	}
	else
	{
		env_set(cmd[1], cmd[2]);
	}
}

void do_cmd_getenv(void)
{
	
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
			int cnt = simple_strtoul(cmd[2], (void*)0, 10);
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
			int freq = simple_strtoul(cmd[2], (void*)0, 10);
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
void inline lcd_usage(void)
{
	printf("LCD Usage:\r\n"
		   "    lcd test\r\n");
}

void do_cmd_lcd(void)
{
	// 真正的lcd命令的操作实现
	// 目前支持的命令有lcd test
	// cmd[0]里面是lcd，cmd[1]里面是test
	if (!strcmp(cmd[1], "test"))
	{
		// lcd test
		lcd_test();
	}
	// ..... 还可以继续扩展
	else
	{
		// 如果一个都没匹配，则打印使用方法
		lcd_usage();
		printf_separator();
	}
}


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
			int counts = simple_strtoul(cmd[2], (void*)0, 10);
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

	strcpy(g_cmdset[4], printenv);
	strcpy(g_cmdset[5], setenv);
	strcpy(g_cmdset[6], HELP);
	
	
	memset((char *)cmd, 0, sizeof(cmd));	
}

// 将用户输入的字符串命令str按照空格分隔成多个字符串，依次放入cmd二维数组中
void cmdsplit(char cmd[][MAX_LEN_PART], const char *str)
{
    int m = 0, n = 0;	// m表示二位数组第一维，n表示第二维
	//如果是设置环境变量，需要特殊处理.
	if (!strncmp(str, setenv, strlen(setenv)))
	{
		int long_string = 0;
        while (*str != '\0')
        {
        	if (*str != ' ')
        	{
        		cmd[m][n] = *str;
        		n++;
        	}
        	else
        	{
				if (long_string != 1)
				{
                    cmd[m][n] = '\0';
        		    n = 0;
        		    m++;
				}
        		
				
				if (m == 2)
				{
					long_string = 1;

					while (isspace(*str))
					{
						str++;
					}
					break; //跳出循环，在下面的 while 做特殊处理.
				}
        	}
        	str++;
        }

		while (*str != '\0')
        {
        	cmd[m][n] = *str;
        	n++;
        	str++;
        }
	}
    else
	{
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
			do_cmd_lcd();			break;
		case 2:		// buzzer	
			do_cmd_buzzer();		break;
		case 3:		// adc	
			do_cmd_adc();			break;
		case 4:		// printenv	
			do_cmd_printenv();		break;
		case 5:		// setenv	
			do_cmd_setenv();		break;
		case 6:		// help	
			print_help();		break;	
		default:
			do_cmd_notfound();		
			print_help();
			break;
	}
}
