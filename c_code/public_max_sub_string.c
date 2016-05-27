#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wtypes.h>


//矩阵法：查找最大子串
//斜率1最多的为最大子串
int max_public_string(char *str1, char *str2, int *index)
{
	int old_count = 0;
	int old_max = 0;
	int count = 0;
	int max = 0;

	while (count < strlen(str1) && count < strlen(str2))
	{
		if (str1[count] == str2[count])
		{
			max++;
		}
		else
		{
			if (old_max < max)
			{
				old_max = max;
				*index = old_count;  
			}

			old_count = count+1;
			max = 0;
		}

		count++;
	}

	return old_max;
}

void public_string(char *str1, char *str2, char *dest)
{
	int i,j;
	int max = 0;
	int index = 0;

	for (i=0; i<strlen(str1); i++)
	{
		int idx = 0;
		int ret = max_public_string(str1+i, str2, &idx);
		if (ret > max)
		{
			max = ret;
			index = idx + i;
		}
	}

	for (j=strlen(str2)-1; j>=0; j--)
	{
		int idx = 0;
		int ret = max_public_string(str1, str2+j, &idx);
		if (ret > max)
		{
			max = ret;
			index = idx;
		}
	}

	printf("max: %d -- index of str1 : %d \n", max, index);
	strncpy(dest, str1+index, max);
}


//测试
char *a = "aacabbdcdeeeeeeeee";
char *b = "aacaaaacabbeee";
int main()
{	
	char buff[100] = {0};
	public_string(a, b, buff);

	printf("max public string: %s \n", buff);

	getchar();
	return 0;
}
