#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wtypes.h>


//����CSV��ʽ
//1������Ƕ��Ÿ���
//2��ȫ�Ƕ��Ų����������洢
//3������"��,����""��������"��""���

//csv�ļ�
//234324.jpg,111,"����""�������""",�����ҵģ���,"112,34",t_xing,""",""",","""""","

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

		//���⴦��
		if (*start == '\"')
		{
			start++; //ȥ�����"
			
			while (start < buffer + len)
			{
				if (*start == '\"')
				{
					start++;
					if (start >= buffer + len)
						break;
					
					//����""�滻Ϊ1��"
					if (*start == '\"')
					{
						temp_buf[buf_count++] = *start;
						start++;
						continue;
					}
					//����
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
		else	//��������
		{
			//�ҵ��¸�����
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

//����
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
