#include "stdio.h"
#include "shell.h"
#include "main.h"

env_t  envset[MAX_ENV_NUM];			// 系统最多支持10个环境变量


void env_init(void)
{
	memset((char *)envset, 0, sizeof(envset));
	// 第一个环境变量
	strcpy(envset[0].env_name, "bootdelay");
	strcpy(envset[0].env_val, "6");
	envset[0].is_used = 1;
	
	// 第二个环境变量
	strcpy(envset[1].env_name, "bootcmd");
	strcpy(envset[1].env_val, "Not found");
	envset[1].is_used = 1;
	
	// 更多的环境变量
}

// 成功找到env则返回0，失败则返回1.
int env_get(const char *pEnv, char *val)
{
	int i;
	
	for (i=0; i<MAX_ENV_NUM; i++)
	{
		if (!envset[i].is_used)
		{
			continue;
		}
		if (!strcmp(envset[i].env_name, pEnv))
		{
			// 找到了环境变量
			strcpy(val, envset[i].env_val);
			return 0;
		}
	}
	return 1;
}

void env_set(const char *pEnv, const char *val)
{
	int i;
	// 先找目前有没有这个环境变量，如果有就直接改值
	for (i=0; i<MAX_ENV_NUM; i++)
	{
		if (!envset[i].is_used)
		{
			continue;
		}
		if (!strcmp(envset[i].env_name, pEnv))
		{
			//如果该变量为 NULL，则取消该环境变量
			if(!strcmp(val, ""))
			{
				envset[i].is_used = 0;
			}
			// 找到了环境变量
			else 
			{
				strcpy(envset[i].env_val, val);
			}

			return;
		}
	}

	// 没找到这个环境变量，则新建即可
	for (i=0; i<MAX_ENV_NUM; i++)
	{
		//如果环境变量名相同，或者该数组元素未被使用，则可以处理当前元素
		if ( ( !strcmp(envset[i].env_name, pEnv)) || !envset[i].is_used )
		{
			//如果该变量为 NULL，则取消该环境变量
			if(!strcmp(val, ""))
			{
				envset[i].is_used = 0;
			}
			else
			{
                strcpy(envset[i].env_name, pEnv);
		        strcpy(envset[i].env_val, val);
		        envset[i].is_used = 1;
			}
		    return;
		}
		
	}
	
	// 找遍了环境变量的数组，还是没有空位，说明已经存满了
	printf("env array is full.\r\n");
}



