#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wtypes.h>


//解析CSV格式
//1、均半角逗号隔开
//2、全角逗号不处理，正常存储
//3、出现"或,则用""括起来，"用""替代

//csv文件
//234324.jpg,111,"好书""理解万岁""",你是我的，好,"112,34",t_xing,""",""",","""""","

void parse_csv(char *buffer)
{
	char temp_buf[128] = {0};
	char buf_count = 0;
	char *start = buffer;
	int len;

	if (buffer == NULL || *buffer == '\0')
		return;

	len = strlen(buffer);

	while (start < buffer + len)
	{
		buf_count = 0;
		memset(temp_buf, 0, sizeof(temp_buf));

		//特殊处理
		if (*start == '\"')
		{
			start++; //去掉外层"
			
			while (start < buffer + len)
			{
				if (*start == '\"')
				{
					start++;
					if (start >= buffer + len)
						break;
					
					//两个""替换为1个"
					if (*start == '\"')
					{
						temp_buf[buf_count++] = *start;
						start++;
						continue;
					}
					//结束
					else if (*start == ',')
					{
						start++;
						break;
					}
				}
				
				temp_buf[buf_count++] = *start;
				start++;
			}
		}
		else	//正常处理
		{
			//找到下个逗号
			while (start < buffer + len)
			{
				if (*start == ',')
					break;
				
				temp_buf[buf_count++] = *start;
				start++;
			}
			
			start++;
		}

		printf("%s \n", temp_buf);
	}
}

//测试
int main()
{	
	char buffer[1024] = {0};
	FILE *file = fopen("D:\\tt.csv", "r");

	if (file)
	{
		char *lines = fgets(buffer, 1024, file);
		if (lines)
		{
			parse_csv(buffer);
		}
	}

	fclose(file);

	getchar();
	return 0;
}
