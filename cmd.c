// 命令解析和命令执行相关的函数
#include "shell.h"
#include "stdio.h"


char g_cmdset[CMD_NUM][MAX_LINE_LENGTH];


// 初始化命令列表
void init_cmd_set(void)
{
	memset((char *)g_cmdset, 0, sizeof(g_cmdset));		// 先全部清零
	strcpy(g_cmdset[0], led);
	strcpy(g_cmdset[1], lcd);
	strcpy(g_cmdset[2], pwm);
}

// 解析命令
void cmd_parser(char *str)
{
	int i;
	
	for (i=0; i<CMD_NUM; i++)
	{
		if (!strcmp(str, g_cmdset[i]))
		{
			// 相等，找到了这个命令，就去执行这个命令所对应的动作。
			printf("your enter is: %s\r\n", str);
			break;
		}	
	}

	if (i >= CMD_NUM)
	{
		// 找遍了命令集都没找到这个命令
		printf("%s is illegal, please try again.\r\n", str);
	}
}

// 执行命令
void cmd_exec(void)
{
	
}

