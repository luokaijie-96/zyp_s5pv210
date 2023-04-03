// 命令解析和命令执行相关的函数
#include "shell.h"
#include "stdio.h"


char g_cmdset[CMD_NUM][MAX_LINE_LENGTH];		// 命令集，存主命令
char cmd[MAX_CMD_PART][MAX_LEN_PART];			// 当前解析出来的命令
int cmd_index = -1;								// 存储解析到的命令是第几个主命令


// 初始化命令列表
void init_cmd_set(void)
{
	memset((char *)g_cmdset, 0, sizeof(g_cmdset));		// 先全部清零
	strcpy(g_cmdset[0], led);
	strcpy(g_cmdset[1], lcd);
	strcpy(g_cmdset[2], pwm);

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
	
    // 第一步，先将用户输入的次命令字符串分割放入cmd中
    cmdsplit(cmd, str);
    
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


// 命令没找到处理方法
void do_cmd_notfound(void)
{
	printf("%s command is illegal, please input again\r\n", cmd[0]);
}

// led命名的处理方法
void do_cmd_led(void)
{
	printf("led cmd\r\n");
}



// 执行命令
void cmd_exec(void)
{
	switch (cmd_index)
	{
		case 0:
			do_cmd_led();			break;
		case 1:
		case 2:
		default:
			do_cmd_notfound();		break;
	}
}