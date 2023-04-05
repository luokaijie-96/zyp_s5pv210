#ifndef __SHELL_H__
#define __SHELL_H__



// 宏定义
#define MAX_LINE_LENGTH		256			// 命令行长度，命令不能超过这个长度

// 宏定义一些标准命令的第一部分
#define led					"led"
#define lcd					"lcd"
#define pwm					"buzzer"
#define adc					"adc"

#define printenv			"printenv"
#define setenv				"setenv"
#define HELP				"help"

#define CMD_NUM				30			// 当前系统定义的命令数 

// 宏定义一个命令相关信息
#define MAX_CMD_PART		10			// 一个命令最多包含几部分			
#define MAX_LEN_PART		256			// 命令的分部分最大长度



// 结构体就是环境变量的模板，将来每一个环境变量就是这个模板的一个实例
#define   MAX_ENV_NUM    30 

// 所有环境变量


typedef struct env
{
	char env_name[MAX_LEN_PART];
	char env_val[MAX_LEN_PART];
	int is_used;			// 标志位，0表示这个环境变量没用，1表示用了
}env_t;
extern env_t envset[MAX_ENV_NUM];


// 全局变量声明
extern char g_cmdset[CMD_NUM][MAX_LINE_LENGTH];
extern int g_isgo;				
extern int g_bootdelay;	 


// 命令解析/执行相关
void init_cmd_set(void);
void cmd_parser(char *str);
void cmd_exec(void);

void print_help();
void adc_usage(void);
void lcd_usage(void);
void buzzer_usage(void);
void led_usage(void);


// 环境变量相关
void env_init(void);
int env_get(const char *pEnv, char *val);
void env_set(const char *pEnv, const char *val);

#endif