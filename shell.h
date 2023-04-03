#ifndef __SHELL_H__
#define __SHELL_H__


// 宏定义
#define MAX_LINE_LENGTH		256			// 命令行长度，命令不能超过这个长度

// 宏定义一些标准命令的第一部分
#define led					"led"
#define lcd					"lcd"
#define pwm					"pwm"
#define CMD_NUM				30			// 当前系统定义的命令数 

// 宏定义一个命令相关信息
#define MAX_CMD_PART		5			// 一个命令最多包含几部分			
#define MAX_LEN_PART		20			// 命令的分部分最大长度



// 全局变量声明
extern char g_cmdset[CMD_NUM][MAX_LINE_LENGTH];

// 命令解析/执行相关
void init_cmd_set(void);
void cmd_parser(char *str);
void cmd_exec(void);



#endif