#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//sunday�㷨
//��BM�㷨���죬��������⡣
//����˼�룺��ƥ������У�ģʽ��������Ҫ��һ��
//Ҫ���������ҽ��бȽϻ��Ǵ���������бȽϣ����ڷ��ֲ�ƥ��ʱ��
//�㷨�����������ܶ���ַ��Խ�����һ����ƥ�䣬�Ӷ������ƥ��Ч�ʡ�

//��ƥ��ʱ�Ĺ��򣺲鿴T�¸��ַ��Ƿ���P�г���
//1��δ���֣���P�ƶ����¸��ַ��ĺ�һλ
//2�����֣���P��T���ո��ַ�����

//eg: ����eδ��P�г���
//T :  a  b  c  e  a  b  d
//P :  a  b  d

//�ƶ�
//T :  a  b  c  e  a  b  d
//P :              a  b  d

//eg: ����b��P�г���
//T :  a  b  c  b  a  b  d
//P :  a  b  d

//�ƶ�
//T :  a  b  c  b  a  b  d
//P :        a  b  d

//��ʼ��next
void init_shift(int *next, int next_len, char *pattern, int pattern_len)
{
	int i;
	assert(next != NULL);
	for (i=0; i<next_len; i++)
	{
		next[i] = pattern_len+1;
	}

	for (i=0; i<pattern_len; i++)
	{
		next[pattern[i]-'a'] = pattern_len-i;
	}
}

//�Ƚ����ַ����Ƿ�ƥ��, 0ƥ�䣬����ƥ��
int match_string(char *text, int text_len, char *pattern, int pattern_len)
{
	assert(text !=NULL && pattern != NULL);
	if (text_len < pattern_len)
		return -1;
	while (pattern_len--)
	{
		if (text[pattern_len] != pattern[pattern_len])
			return -1;
	}

	return 0;
}

//ƥ�䷵��0
int sunday_match(char *text, char *pattern)
{
	int ret = -1;
	int index = 0;
	int next['z'-'a'+1];
	int text_len = strlen(text);
	int pattern_len = strlen(pattern);

	assert(text != NULL && pattern != NULL);

	init_shift(next, sizeof(next)/sizeof(next[0]), pattern, pattern_len);

	while (index < text_len)
	{
		ret = match_string(text+index, text_len-index, pattern, pattern_len);
		if (ret != 0)
			index += next[text[index+pattern_len]-'a'];
		else
			break;
	}

	return ret;
}

int main(int argc, char *argv[])
{
    char src[]="abcdacdaahfacabcdabcdeaa";  
    char des[]="abcde"; 
	int ret;

	ret = sunday_match(src, des);
	printf("%d\n", ret);
	
	getchar();
	return 0;
}